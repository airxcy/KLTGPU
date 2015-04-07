#include"trackers/utils.h"

int getLineIdx(std::vector<int>& x_idx,std::vector<int>&  y_idx,int* PointA,int* PointB)
{
    /*
    draw a line from PointA to PointB store postions in x_idx y_idx
    return length of the line always >0
    reference Bresenham's line algorithm
    you need to free memory of x_idx y_idx your self
    */
    int idx_N=0,startx=PointA[0],starty=PointA[1],endx=PointB[0],endy=PointB[1];
    int diffx=(endx-startx),diffy=(endy-starty);
    int dx=(diffx > 0) - (diffx < 0), dy=(diffy > 0) - (diffy < 0);
        diffx=diffx*dx,diffy=diffy*dy;
    int x=startx,y=starty;
    int step,incre,err,thresherr;
    int *increter,*steper;
    if(diffx>=diffy)
    {
        err=diffy;
        thresherr=diffx/2;
        increter=&y;
        incre=dy;
        steper=&x;
        step=dx;
        idx_N=diffx;
    }
    else
    {
        err=diffx;
        thresherr=diffy/2;
        increter=&x;
        incre=dx;
        steper=&y;
        step=dy;
        idx_N=diffy;
    }
    int toterr=0,i=0;
    for(i=0;i<idx_N;i++)
    {
        x_idx.push_back(x);
        y_idx.push_back(y);
        (*steper)+=step;
        toterr+=err;
        if((toterr)>=thresherr)
        {
            toterr=toterr-idx_N;
            (*increter)+=incre;
        }
    }
    x_idx.push_back(PointB[0]);
    y_idx.push_back(PointB[1]);
    return idx_N+1;
}
int getLineProp(std::vector<int>& x_idx,std::vector<int>&  y_idx,int* PointA,int* PointB,double linedist)
{
    int xA=PointA[0],yA=PointA[1],xB=PointB[0],yB=PointB[1];
    int linelen = linedist+0.5+1;
    double xstep=(xB-xA)/linedist;
    double ystep=(yB-yA)/linedist;
    for(int i=0;i<linelen;i++)
    {
        int x = xA+i*xstep+0.5;
        int y = yA+i*ystep+0.5;
        x_idx.push_back(x);
        y_idx.push_back(y);
    }
    return linelen;
}
