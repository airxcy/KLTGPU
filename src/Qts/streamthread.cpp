#include "Qts/streamthread.h"

#include "Qts/viewqt.h"
#include "Qts/modelsqt.h"
#include "trackers/utils.h"
#include <iostream>
#include <fstream>
//#include <stdlib.h>

//#include <direct.h>
#include "Qts/mainwindow.h"
#include <opencv2/opencv.hpp>
#include <ctime>

#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QStringList>

using namespace cv;

VideoCapture cap;
Mat frame;
float fps=0;
char strbuff[100];
QDir qdirtmp;
Mat gray;
StreamThread::StreamThread(QObject *parent) : QThread(parent)
{
    restart = false;
    abort = false;
    pause = false;
    bufflen=0;
    trkscene=NULL;
    framebuff=NULL;
    inited=false;
}
StreamThread::~StreamThread()
{
    abort = true;
    cv0.wakeAll();
    wait();
}

bool StreamThread::init()
{
    restart=false,abort=false,pause=false;
    bufflen=0;

    if(!cap.isOpened())
    {
        cap.open(vidfname);
        std::cout<<"reopened"<<std::endl;
    }
    cap.set(CV_CAP_PROP_POS_FRAMES,0);
    frameidx=0;
    cap>>frame;
    fps=0;
    delay=25;
    bufflen=delay+10;
    cvtColor(frame,frame,CV_BGR2RGB);
    framewidth=frame.size[1],frameheight=frame.size[0];
    cvtColor(frame,gray,CV_BGR2GRAY);

    if(framebuff==NULL)
    {
        framebuff = new FrameBuff();
        framebuff->init(frame.elemSize(),framewidth,frameheight,bufflen);
    }
    else
        framebuff->clear();

    frameByteSize=frame.size[0]*frame.size[1]*frame.elemSize();
    framebuff->updateAFrame(frame.data);
    frameptr=framebuff->cur_frame_ptr;
    tracker = new KLTtracker();
    tracker->init(10,framewidth,frameheight);
    tracker->selfinit(gray.data);
    inited=true;
    return cap.isOpened();
}
void StreamThread::streaming()
{
    forever
    {
        if(init())
        {
            emit initSig();
            frameidx=0;
            int fcounter=0;
            std::clock_t start = std::clock();
            double duration;
            while(!frame.empty())
            {
                cap >> frame;
                if(frame.empty())
                    break;
                cvtColor(frame,gray,CV_BGR2GRAY);
                cvtColor(frame,frame,CV_BGR2RGB);
                tracker->updateAframe(gray.data,frameidx);
                //framebuff->updateAFrame(frame.data);
                framebuff->updateAFrame(tracker->kltdense);
                frameptr=framebuff->cur_frame_ptr;
                frameidx++;
                //std::cout<<frameidx<<std::endl;
                trkscene->update();
                fcounter++;
                duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
                if(duration>=1)
                {
                    fps=fcounter/duration;
                    start=std::clock() ;
                    fcounter=0;
                }

            }
        }
        else
        {
            //emit debug( "init Failed");
        }
        trkscene->clear();
        inited=false;
    }
}
void StreamThread::run()
{
    streaming();
}

void StreamThread::streamStart(std::string & filename)
{
    QMutexLocker locker(&mutex);
    //QMessageBox::question(NULL, "Test", "msg",QMessageBox::Ok);
    if (!isRunning()) {
        vidfname=filename;
        start(InheritPriority);
    }
    else
    {
        restart=true;
    }
}
