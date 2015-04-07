#include "Qts/viewqt.h"
#include "Qts/modelsqt.h"
#include "Qts/streamthread.h"

#include <iostream>

#include <QPainter>
#include <QBrush>
#include <QPixmap>
#include <cmath>
#include <QGraphicsSceneEvent>
#include <QMimeData>
#include <QByteArray>
#include <QFont>
char viewstrbuff[200];
void DefaultScene::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    emit clicked(event);
}
void DefaultScene::drawBackground(QPainter * painter, const QRectF & rect)
{
    QPen pen;
    QFont txtfont("Roman",40);
    txtfont.setBold(true);
    pen.setColor(QColor(255,255,255));
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setWidth(10);
    painter->setPen(QColor(243,134,48,150));
    painter->setFont(txtfont);
    painter->drawText(rect, Qt::AlignCenter,"打开文件\nOpen File");
}
TrkScene::TrkScene(const QRectF & sceneRect, QObject * parent):QGraphicsScene(sceneRect, parent)
{
    streamThd=NULL;
}
TrkScene::TrkScene(qreal x, qreal y, qreal width, qreal height, QObject * parent):QGraphicsScene( x, y, width, height, parent)
{
    streamThd=NULL;
}
void TrkScene::drawBackground(QPainter * painter, const QRectF & rect)
{
    //std::cout<<streamThd->inited<<std::endl;
    if(streamThd!=NULL&&streamThd->inited)
    {
        updateFptr(streamThd->frameptr, streamThd->frameidx);
    }
    painter->setBrush(bgBrush);
    painter->drawRect(rect);

    if(streamThd!=NULL&&streamThd->inited)
    {
        painter->setPen(Qt::red);
        painter->setFont(QFont("System",20,2));
        painter->drawText(rect, Qt::AlignLeft|Qt::AlignTop,QString::number(streamThd->fps));
        std::vector<TrackBuff>& klttrkvec=streamThd->tracker->trackBuff;
        for(int i=0;i<klttrkvec.size();i++)
        {

            TrackBuff& klttrk= klttrkvec[i];
            unsigned char r=feat_colos[i%6][0],g=feat_colos[i%6][1],b=feat_colos[i%6][2];
            double x0,y0,x1,y1;
            int startidx=std::max(1,klttrk.len-5);
            for(int j=startidx;j<klttrk.len;j++)
            {
                 x1=klttrk.getPtr(j)->x,y1=klttrk.getPtr(j)->y;
                 x0=klttrk.getPtr(j-1)->x,y0=klttrk.getPtr(j-1)->y;
                //int indcator =(j+30)>255;
                //int alpha = indcator*255+(1-indcator)*(j+30);
                //double linedist = sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0));
                //if(linedist>50)
                    //std::cout<<j<<","<<klttrk.len<<std::endl;
                    //painter->drawText(x0,y0,QString::number(j));
                painter->setPen(QColor(r,g,b));//,alpha));
                painter->drawLine(x0,y0,x1,y1);

            }
                //painter->drawText(x1,y1,QString::number(klttrk.len));
        }

    }

    //update();
    //views().at(0)->update();
}
void TrkScene::updateFptr(unsigned char * fptr,int fidx)
{
    bgBrush.setTextureImage(QImage(fptr,streamThd->framewidth,streamThd->frameheight,QImage::Format_RGB888));
    frameidx=fidx;
    //std::cout<<frameidx<<std::endl;
}
void TrkScene::clear()
{
    bgBrush.setStyle(Qt::NoBrush);
}
