#ifndef VIEWQT
#define VIEWQT

#include <QGraphicsScene>
#include <QGraphicsView>
#include <vector>
#include <QThread>
#include "Qts/modelsqt.h"
class StreamThread;
class DefaultScene : public QGraphicsScene
{
    Q_OBJECT
public:
    DefaultScene(const QRectF & sceneRect, QObject * parent = 0):QGraphicsScene(sceneRect, parent)
    {

    }
    DefaultScene(qreal x, qreal y, qreal width, qreal height, QObject * parent = 0):QGraphicsScene( x, y, width, height, parent)
    {

    }
    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event ) Q_DECL_OVERRIDE;
    virtual void drawBackground(QPainter * painter, const QRectF & rect) Q_DECL_OVERRIDE;
signals:
    void clicked(QGraphicsSceneMouseEvent * event);
};
class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(QGraphicsScene *scene) : QGraphicsView(scene)
    {
        setMouseTracking(true);
        setAcceptDrops(true);
        setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    }
    GraphicsView(QGraphicsScene *scene, QWidget * parent = 0) : QGraphicsView(scene,parent)
    {
        setMouseTracking(true);
        setAcceptDrops(true);
        setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    }
};

class TrkScene :public QGraphicsScene
{
    Q_OBJECT
public:
    TrkScene(const QRectF & sceneRect, QObject * parent = 0);
    TrkScene(qreal x, qreal y, qreal width, qreal height, QObject * parent = 0);
    StreamThread* streamThd;
    QBrush bgBrush;
    QFont txtfont;
    int frameidx;
    virtual void drawBackground(QPainter * painter, const QRectF & rect) Q_DECL_OVERRIDE;
    void updateFptr(unsigned char * fptr,int fidx);
    void clear();
};
#endif // VIEWQT

