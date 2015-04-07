#include "trackers/klttracker.h"

#include <cmath>
#include <fstream>
#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/gpu/gpu.hpp>
using namespace cv;
using namespace std;
#define PI 3.14159265

Mat corners,prePts,nextPts,status,eigenvec;
cv::gpu::GoodFeaturesToTrackDetector_GPU detector;
cv::gpu::PyrLKOpticalFlow tracker;
cv::gpu::GpuMat gpuGray, gpuPreGray, gpuCorners, gpuPrePts, gpuNextPts,gpuStatus,gpuEigenvec,gpuDenseX,gpuDenseY,gpuDenseXC,gpuDenseYC;
Mat denseX,denseY,denseRGB;
bool checkCurve(TrackBuff &strk)
{
    double maxdist = .0, dtmp = .0, totlen = .0;
    TrkPts * aptr = strk.getPtr(0), *bptr = aptr;
    REAL xa = aptr->x, ya = aptr->y, xb = strk.cur_frame_ptr->x, yb = strk.cur_frame_ptr->y;
    double displc = sqrt(pow(xb - xa, 2.0) + pow(yb - ya, 2.0));
    for (int j = 1; j <strk.len; ++j)
    {
        aptr = strk.getPtr(j - 1);
        bptr = strk.getPtr(j);
        dtmp = sqrt(pow(bptr->x - aptr->x, 2.0) + pow(bptr->y - aptr->y, 2.0));
        totlen += dtmp;
        if (dtmp>maxdist)maxdist = dtmp;
    }
    return (strk.len > 20 && totlen*0.5 > displc);
}
int getAvgVec(FeatBuff& trk,REAL * dir,int dly)
{
    if(trk.len<=8)return 0;
    int startidx=0,endidx=trk.len-1;
    FeatPts * aptr = trk.getPtr(startidx);
    FeatPts * bptr = trk.getPtr(endidx);
    dir[0]=bptr->x - aptr->x;
    dir[1]=bptr->y - aptr->y;
    REAL norm=sqrt(dir[0]*dir[0]+dir[1]*dir[1]);
    dir[0]/=norm;
    dir[1]/=norm;
    return 1;
    /*
    for (int j = 1; j <strk.len; ++j)
    {
        aptr = strk.getPtr(j - 1);
        bptr = strk.getPtr(j);
        dtmp = sqrt(pow(bptr->x - aptr->x, 2.0) + pow(bptr->y - aptr->y, 2.0));
        totlen += dtmp;
        if (dtmp>maxdist)maxdist = dtmp;
    }
    */
}
int PredictTrk(FeatPts* point,FeatBuff& trk)
{
    if(trk.len<2)
    {
        memcpy(point,trk.cur_frame_ptr,sizeof(FeatPts));
        return 0;
    }
    int startidx=max(trk.len-8,0),endidx=trk.len-1;
    double len=endidx-startidx+1;
    FeatPts* aptr=trk.getPtr(startidx);
    FeatPts* bptr=trk.getPtr(endidx);
    point->x=(bptr->x-aptr->x)/len+bptr->x;
    point->y=(bptr->y-aptr->y)/len+bptr->y;
    point->t=bptr->t+1;
}

int KLTtracker::init(int bsize,int w,int h)
{
    nFeatures = 1000;
    nSearch=2000;
    trackBuff=std::vector<TrackBuff>(nFeatures);
    isTracking=new int[nFeatures];
	for (int i=0;i<nFeatures;i++)
	{
		trackBuff[i].init(1,100);
        isTracking[i]=0;
	}

	frame_width = w;
	frame_height = h;
	frameidx=0;
    goodNewPts.init(1,nSearch);

    detector= gpu::GoodFeaturesToTrackDetector_GPU(nSearch,0.0001,3,3);
    tracker = gpu::PyrLKOpticalFlow();
    tracker.winSize=Size(3,3);
    tracker.maxLevel=3;
    tracker.iters=10;
    gpuGray=gpu::GpuMat(frame_height, frame_width, CV_8UC1 );
    gpuPreGray=gpu::GpuMat(frame_height, frame_width, CV_8UC1 );
    denseX=Mat(frame_height, frame_width, CV_8UC1);
    denseY=Mat(frame_height, frame_width, CV_8UC1);
    kltdense=new unsigned char[frame_height* frame_width*3];
    memset(kltdense,0,frame_height* frame_width*3);

	gt_inited = false;
	return 1;
}

int KLTtracker::selfinit(unsigned char* framedata)
{
	curframedata=framedata;
    Mat curframe(frame_height,frame_width,CV_8UC1,framedata);
    gpuGray.upload(curframe);
    gpuPreGray.upload(curframe);
    detector(gpuGray, gpuCorners);
    gpuCorners.download(corners);
    gpuCorners.copyTo(gpuPrePts);
    for (int k = 0; k < nFeatures; k++)
    {
        Vec2f p = corners.at<Vec2f>(k);
        pttmp.x = (PntT)(p[0] + 0.5);
        pttmp.y = (PntT)(p[1]+ 0.5);
        pttmp.t = frameidx;
        trackBuff[k].updateAFrame(&pttmp);
        isTracking[k]=1;
    }
	return true;
}

int KLTtracker::checkFG(int x,int y)
{
    return (int)bgdata[y*frame_width+x];
}

void KLTtracker::updateFGMask(unsigned char* bgptr)
{
	bgdata=bgptr;
}

bool KLTtracker::checkTrackMoving(TrackBuff &strk)
{
	bool isTrkValid = true;

    int Movelen=10,minlen=5,startidx=max(strk.len-Movelen,0);
    if(strk.len>Movelen)
    {
        double maxdist = .0, dtmp = .0,totlen=.0;

        TrkPts* aptr = strk.getPtr(startidx), *bptr = aptr;
        //int xa=(*aptr),ya=(*(aptr+1)),xb=*(strk.cur_frame_ptr),yb=*(strk.cur_frame_ptr+1);
        PntT xa=aptr->x,ya=aptr->y,xb=strk.cur_frame_ptr->x,yb=strk.cur_frame_ptr->y;
        double displc=sqrt( pow(xb-xa, 2.0) + pow(yb-ya, 2.0));
        /*
        for(int posi=0;posi<strk.len;posi++)
        {
            bptr=strk.getPtr(posi);
            xb=bptr->x,yb=bptr->y;
            dtmp = sqrt( pow(xb-xa, 2.0) + pow(yb-ya, 2.0));
            totlen+=dtmp;
            if (dtmp > maxdist && posi>=startidx) maxdist = dtmp;
            xa=xb,ya=yb;
        }
        */
        //if(strk.len>100&&totlen*0.5>displc){strk.isCurved=true;}
        if((strk.len -startidx)*0.2>displc)
        {
            isTrkValid = false;
        }
        //if (maxdist < 1.4 && strk.len>30){isTrkValid = false;}
        //if (maxdist <=0.1 && strk.len>=minlen){isTrkValid = false;}
    }
	return isTrkValid;
}

int KLTtracker::updateAframe(unsigned char* framedata,int fidx)
{
    frameidx=fidx;
	curframedata=framedata;
    gpuGray.copyTo(gpuPreGray);
    Mat curframe(frame_height,frame_width,CV_8UC1,framedata);
    gpuGray.upload(curframe);
    /*
    tracker.dense(gpuPreGray,gpuGray,gpuDenseX,gpuDenseY);
    gpu::multiply(gpuDenseX,255.0/1000,gpuDenseX);
    gpu::add(gpuDenseX,127,gpuDenseX);
    gpuDenseX.convertTo(gpuDenseXC,CV_8UC1);
    gpu::max(gpuDenseXC,0,gpuDenseXC);
    gpu::min(gpuDenseXC,255,gpuDenseXC);
    gpu::multiply(gpuDenseY,255.0/1000,gpuDenseY);
    gpu::add(gpuDenseY,127,gpuDenseY);
    gpuDenseY.convertTo(gpuDenseYC,CV_8UC1);
    gpu::max(gpuDenseYC,0,gpuDenseYC);
    gpu::min(gpuDenseYC,255,gpuDenseYC);
    gpuDenseXC.download(denseX);
    gpuDenseYC.download(denseY);
    for(int pixi=0;pixi<frame_width*frame_height;pixi++)
    {
        //kltdense[pixi*3]=255-*(denseY.data+pixi);
        kltdense[pixi*3+2]=*(denseY.data+pixi);
    }
    */

    tracker.sparse(gpuPreGray, gpuGray, gpuPrePts, gpuNextPts, gpuStatus, &gpuEigenvec);
    gpuStatus.download(status);
    gpuNextPts.download(nextPts);
    gpuPrePts.download(prePts);
    detector(gpuGray, gpuCorners);
    gpuCorners.download(corners);

    goodNewPts.clear();
    for(int i=0;i<nSearch;i++)
    {
        bool goodNew=true;
        Vec2f cnrp = corners.at<Vec2f>(i);
        for(int k = 0; k < nFeatures; k++)
        {
            int oldx=trackBuff[k].cur_frame_ptr->x,oldy=trackBuff[k].cur_frame_ptr->y;
            double dx=cnrp[0]-oldx,dy=cnrp[1]-oldy;
            //double dist= sqrt(dx*dx+dy*dy);
            double dist=abs(dx)+abs(dy);
            if(dist<20)
            {
                goodNew=false;
            }
        }
        if(goodNew)
        {
            goodNewPts.updateAFrame(&i);
        }
    }

    int addidx=0,counter=0;
	for (int k = 0; k < nFeatures; k++)
	{
        int statusflag = status.at<int>(k);
        Vec2f trkp = nextPts.at<Vec2f>(k);
        bool lost=false;
        if ( statusflag)
		{
            Vec2f pre=prePts.at<Vec2f>(k),cur=nextPts.at<Vec2f>(k);
            int prex=trackBuff[k].cur_frame_ptr->x, prey=trackBuff[k].cur_frame_ptr->y;
            pttmp.x = (PntT)(trkp[0] + 0.5);
            pttmp.y = (PntT)(trkp[1] + 0.5);
            pttmp.t = frameidx;
            trackBuff[k].updateAFrame(&pttmp);
            double trkdist=abs(prex-pttmp.x)+abs(prey-pttmp.y),ofdist=abs(pre[0]-cur[0])+abs(pre[1]-cur[1]);

            isTracking[k]=1;
			bool isMoving=checkTrackMoving(trackBuff[k]);
            if (!isMoving||(trackBuff[k].len>1&&trkdist>20))
			{
                //trackBuff[k].clear();
                lost=true;
                isTracking[k]=0;
			}
		}
        else
        {
            //trackBuff[k].clear();
            counter++;
            lost=true;
            isTracking[k]=0;
		}
        if(lost)
        {

            trackBuff[k].clear();
            if(addidx<goodNewPts.len)
            {
                int newidx=*(goodNewPts.getPtr(addidx++));
                Vec2f cnrp = corners.at<Vec2f>(newidx);
                pttmp.x = (PntT)(cnrp[0] + 0.5);
                pttmp.y = (PntT)(cnrp[1] + 0.5);
                pttmp.t = frameidx;
                trackBuff[k].updateAFrame(&pttmp);
                nextPts.at<Vec2f>(k)=cnrp;
                isTracking[k]=1;
            }
        }
	}
    std::cout<<counter<<","<<goodNewPts.len<<","<<addidx<<std::endl;
    gpuPrePts.upload(nextPts);
	return 1;
}

