#include "Qts/modelsqt.h"
#include <Qts/viewqt.h>

#include <iostream>

#include <QPainter>
#include <QGraphicsSceneEvent>
#include <QDrag>
#include <QMimeData>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsWidget>

unsigned char feat_colos[6][3] =
{
    {0,255,0},
    {0,0,255},
    {255,255, 0},
    {255,0,255},
    {0,255,255},
    {255,0,0},
};
BBox::BBox(QGraphicsItem * parent):QGraphicsItem(parent)
{
    vtx[0]=10, vtx[1]=10, vtx[2]=200, vtx[3]=200;
    bbid=-1;
}
BBox::BBox(double l,double t,double r,double b,QGraphicsItem * parent):QGraphicsItem(parent)
{
    vtx[0]=l, vtx[1]=t, vtx[2]=r, vtx[3]=b;
    bbid=-1;
}
QRectF BBox::boundingRect() const
{
    return QRectF(vtx[0], vtx[1], vtx[2]-vtx[0], vtx[3]-vtx[1]);
}
void BBox::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    prepareGeometryChange();
    painter->setPen(linePen);
    painter->setBrush(inBrush);
    painter->drawRect(boundingRect());
    painter->setFont(txtFont);
    painter->drawText(QRectF (vtx[0], vtx[1],12,12), Qt::AlignRight|Qt::AlignTop,txt);
}
void BBox::updateVtx(double l,double t,double r,double b)
{
    //prepareGeometryChange();
    vtx[0]=l, vtx[1]=t, vtx[2]=r, vtx[3]=b;
    //prepareGeometryChange();
}
void BBox::setClr(unsigned char r,unsigned char g,unsigned char b)
{
    rgb[0]=r,rgb[1]=g,rgb[2]=b;
    linePen.setColor(QColor(r,g,b));
}
DragVtx::DragVtx(QGraphicsItem * parent):QGraphicsItem(parent)
{
    setFlag(ItemIsMovable);
    selected=false;
    coord[0]=0,coord[1]=0,range=3;
    dotPen=QPen(Qt::white);
    setCursor(Qt::CrossCursor);
}
DragVtx::DragVtx(int x, int y,QGraphicsItem * parent):QGraphicsItem(parent)
{
    setFlag(ItemIsMovable);
    selected=false;
    coord[0]=x,coord[1]=y,range=3;
    dotPen=QPen(Qt::white);
    setCursor(Qt::CrossCursor);
}
QRectF DragVtx::boundingRect() const
{
    return QRectF(coord[0]-range,coord[1]-range,2*range+1,2*range+1);
}
void DragVtx::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    prepareGeometryChange();
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(255,255,255));
    painter->drawRect(boundingRect());
    painter->setPen(dotPen);
    painter->drawPoint(coord[0],coord[1]);
}
void DragVtx::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if(event->button()==Qt::LeftButton)
    {
        selected=true;
    }
}
void DragVtx::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        selected=false;
        setCoord(event->pos().x(),event->pos().y());
    }
    ungrabMouse();
}
void DragVtx::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (!(event->buttons() & Qt::LeftButton))
    return;
    if(!selected)return;
    setCoord(event->pos().x(),event->pos().y());
}
void DragVtx::setCoord(int x,int y)
{
    coord[0]=x,coord[1]=y;
}


DragBBox::DragBBox(QGraphicsItem * parent):QGraphicsItem(parent)
{
    //setFlag(ItemIsMovable);
    vtx = std::vector<DragVtx*>(2);
    vtx[0]=new DragVtx(this);
    vtx[1]=new DragVtx(this);
    selected=false;
    isGood=false;
}
DragBBox::DragBBox(int l, int t,int r,int b,QGraphicsItem * parent):QGraphicsItem(parent)
{
    //setFlag(ItemIsMovable);
    vtx = std::vector<DragVtx*>(2);
    vtx[0]=new DragVtx(l,t,this);
    vtx[1]=new DragVtx(r,b,this);
    selected=false;
    isGood=false;
}
QRectF DragBBox::boundingRect() const
{
    return QRectF(QPointF(vtx[0]->coord[0],vtx[0]->coord[1]),QPointF(vtx[1]->coord[0],vtx[1]->coord[1]));
}
void DragBBox::setClr(unsigned char r,unsigned char g,unsigned char b)
{
    rgb[0]=r,rgb[1]=g,rgb[2]=b;
    rectBrush.setColor(QColor(r,g,b,10));
    rectBrush.setStyle(Qt::SolidPattern);
    linepen.setColor(QColor(r,g,b,100));
    //linepen.setWidth(1);
}
void DragBBox::setCoord(int t,int l,int r,int b)
{
    vtx[0]->setCoord(t,l);
    vtx[1]->setCoord(r,b);
}
void DragBBox::focused()
{
    linepen.setColor(QColor(rgb[0],rgb[1],rgb[2],255));
    linepen.setWidth(2);
    setZValue(1);
}
void DragBBox::unfocused()
{
    linepen.setColor(QColor(rgb[0],rgb[1],rgb[2],100));
    linepen.setWidth(1);
    setZValue(0);
}
void DragBBox::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    prepareGeometryChange();
    painter->setBrush(rectBrush);
    painter->setPen(linepen);
    painter->drawRect(boundingRect());
    painter->setPen(QColor(rgb[0],rgb[1],rgb[2]));
    painter->setFont(QFont("Times",20));
    painter->drawText(boundingRect(), Qt::AlignCenter|Qt::AlignCenter,txt);
}

void DragBBox::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if(event->button()==Qt::LeftButton)
    {
        selected=true;
        x1=event->pos().x()-vtx[0]->coord[0];
        y1=event->pos().y()-vtx[0]->coord[1];
        x2=event->pos().x()-vtx[1]->coord[0];
        y2=event->pos().y()-vtx[1]->coord[1];
        if(trkscene!=NULL)
        {
            //trkscene->dragBBclicked(bbid);
            //emit clicked(bbid);
        }
    }
}
void DragBBox::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton&&selected)
    {
        selected=false;
        vtx[0]->setCoord(event->pos().x()-x1,event->pos().y()-y1);
        vtx[1]->setCoord(event->pos().x()-x2,event->pos().y()-y2);
    }
    ungrabMouse();
}

void DragBBox::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (!(event->buttons() & Qt::LeftButton))
    return;
    if(!selected)return;
    vtx[0]->setCoord(event->pos().x()-x1,event->pos().y()-y1);
    vtx[1]->setCoord(event->pos().x()-x2,event->pos().y()-y2);
}
void DragBBox::getltrb(int& l,int& t,int& r,int& b)
{
    if(vtx[0]->coord[0]<vtx[1]->coord[0])
        l = vtx[0]->coord[0],r=vtx[1]->coord[0];
    else
        l = vtx[1]->coord[0],r=vtx[0]->coord[0];
    if(vtx[0]->coord[1]<vtx[1]->coord[1])
        t = vtx[0]->coord[1],b=vtx[1]->coord[1];
    else
        t = vtx[1]->coord[1],b=vtx[0]->coord[1];
}
void DragBBox::clone(DragBBox*& abox)
{
    int l=vtx[0]->coord[0],t=vtx[0]->coord[1],r=vtx[1]->coord[0],b=vtx[1]->coord[1];
    if(abox==NULL)
    {
        abox = new DragBBox(l,t,r,b);
    }
    abox->bbid=bbid;
    memcpy(abox->txt,txt,100);
    memcpy(abox->rgb,rgb,3);
    abox->vtx[0]->setCoord(l,t);
    abox->vtx[1]->setCoord(r,b);
}
void DragBBox::setVtx(int l,int t,int r,int b)
{
    prepareGeometryChange();
    vtx[0]->setCoord(l,t);
    vtx[1]->setCoord(r,b);
}


DragDots::DragDots(QGraphicsItem * parent):QGraphicsItem(parent)
{
    //setFlag(ItemIsMovable);
    selected=false;
    isGood=false;
    range=10;
    setCursor(Qt::CrossCursor);
    //txtfont=QFont("Minecrafter Alt",1.5*range);
    txtfont=QFont("Arial",1.5*range,100);
    ismovable=false;
    recttmp=new QRectF();
    baseRange=10;
}
DragDots::DragDots(int x, int y,QGraphicsItem * parent):QGraphicsItem(parent)
{
    //setFlag(ItemIsMovable);
    coord[0]=x,coord[1]=y;
    selected=false;
    isGood=false;
    range=10;
    setCursor(Qt::CrossCursor);
    //txtfont=QFont("Minecrafter Alt",1.5*range);
    txtfont=QFont("Arial",1.5*range,100);
    ismovable=false;
    recttmp=new QRectF();
    baseRange=10;
}
QRectF DragDots::boundingRect() const
{
    //prepareGeometryChange();
    return QRectF(coord[0]-range,coord[1]-range,2*range+1,2*range+1);
}
void DragDots::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    prepareGeometryChange();
    painter->setBrush(rectBrush);
    painter->setPen(linepen);
    painter->drawRect(boundingRect());
    painter->setPen(QColor(rgb[0],rgb[1],rgb[2],200));
    txtfont.setPixelSize(1.5*range);
    painter->setFont(txtfont);
    //painter->drawText(boundingRect(), Qt::AlignCenter|Qt::AlignCenter,txt);
    painter->drawText(boundingRect(), Qt::AlignCenter|Qt::AlignCenter|Qt::TextDontClip,txt);
    //update();
}
void DragDots::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if(event->button()==Qt::LeftButton&&ismovable)
    {
        selected=true;
        x1=event->pos().x()-coord[0];
        y1=event->pos().y()-coord[1];
    }
    else if(event->button()==Qt::LeftButton&&!ismovable)
    {
        if(trkscene!=NULL)
        {
            //trkscene->dragBBclicked(pid);
            //emit clicked(pid);
        }
    }
}
void DragDots::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (!(event->buttons() & Qt::LeftButton))
    return;
    if(!selected)return;
    setCoord(event->pos().x()-x1,event->pos().y()-y1);
}
void DragDots::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton&&selected)
    {
        selected=false;
        setCoord(event->pos().x()-x1,event->pos().y()-y1);
    }
    ungrabMouse();
}
void DragDots::setCoord(int x,int y)
{
    //std::cout<<x<<","<<y<<std::endl;
    //prepareGeometryChange();
    range=baseRange*(y+500)*0.7/720;
    coord[0]=x,coord[1]=y;
    //update();
}
void DragDots::setClr(unsigned char r,unsigned char g,unsigned char b)
{
    rgb[0]=r,rgb[1]=g,rgb[2]=b;
    if(ismovable)
    {
        rectBrush.setColor(QColor(r,g,b,50));
        rectBrush.setStyle(Qt::SolidPattern);
        linepen.setStyle(Qt::SolidLine);
        txtfont.setBold(true);
        linepen.setColor(QColor(r,g,b,255));
    }
    else
    {
        rectBrush.setColor(QColor(r,g,b,0));
        rectBrush.setStyle(Qt::SolidPattern);
        linepen.setStyle(Qt::NoPen);
        linepen.setColor(QColor(r,g,b,150));
    }
}
void DragDots::focused()
{
    range++;
    rectBrush.setColor(QColor(rgb[0],rgb[1],rgb[2],100));
    setZValue(1);
}
void DragDots::unfocused()
{
    range--;
    rectBrush.setColor(QColor(rgb[0],rgb[1],rgb[2],1));
    setZValue(0);
}
void DragDots::lit()
{
    baseRange=11;
    rectBrush.setColor(QColor(rgb[0],rgb[1],rgb[2],100));
    linepen.setStyle(Qt::SolidLine);
}
void DragDots::unlit()
{
    baseRange=10;
    rectBrush.setColor(QColor(rgb[0],rgb[1],rgb[2],0));
    linepen.setStyle(Qt::NoPen);
}
Anchor::Anchor(int x, int y, QGraphicsItem *parent):Anchor(parent)
{
    coord[0]=x,coord[1]=y;
}
Anchor::Anchor(QGraphicsItem *parent):QGraphicsItem(parent)
{
    coord[0]=0,coord[1]=0;
    pid=-1;
    range=10;
    txtfont=QFont("Minecrafter Alt",1.5*range);
    selected=false;
}
QRectF Anchor::boundingRect() const
{
    return QRectF(coord[0]-range,coord[1]-range,2*range+1,2*range+1);
}
void Anchor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //prepareGeometryChange();
    painter->setBrush(rectBrush);
    painter->setPen(linepen);
    painter->drawRect(boundingRect());
}
void Anchor::setCoord(int x,int y)
{
    prepareGeometryChange();
    coord[0]=x,coord[1]=y;
}
void Anchor::setX(int x)
{
    prepareGeometryChange();
    coord[0]=x;
}
void Anchor::setClr(unsigned char r,unsigned char g,unsigned char b)
{
    rgb[0]=r,rgb[1]=g,rgb[2]=b;
    rectBrush.setColor(QColor(r,g,b,100));
    rectBrush.setStyle(Qt::SolidPattern);
    linepen.setStyle(Qt::SolidLine);
    linepen.setColor(QColor(r,g,b,255));
}
void Anchor::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if(event->button()==Qt::LeftButton)
    {
        selected=true;
        x1=event->pos().x()-coord[0];
        y1=event->pos().y()-coord[1];
    }
}
void Anchor::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (!(event->buttons() & Qt::LeftButton))
    return;
    if(!selected)return;
    //coord[0]=event->pos().x()-x1;
    setX(event->pos().x()-x1);
}
void Anchor::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton&&selected)
    {
        selected=false;
        //coord[0]=event->pos().x()-x1;
        setX(event->pos().x()-x1);
        //setCoord(event->pos().x()-x1,event->pos().y()-y1);
    }
    ungrabMouse();
}
