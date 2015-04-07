#ifndef MODELSQT
#define MODELSQT
#include <QGraphicsItem>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QGraphicsObject>
extern unsigned char feat_colos[6][3];
class TrkScene;
class BBox : public QGraphicsItem
{
public:
    BBox(double l,double t,double r,double b,QGraphicsItem * parent=0);
    BBox(QGraphicsItem * parent=0);
    QPen linePen;
    QBrush inBrush;
    QFont txtFont;
    int bbid;
    char txt[100];
    unsigned char rgb[3];
    double vtx[4];
    virtual QRectF boundingRect() const Q_DECL_OVERRIDE;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
    void updateVtx(double l,double t,double r,double b);
    void setClr(unsigned char r,unsigned char g,unsigned char b);
};

class DragVtx :public QGraphicsItem
{
public:
    DragVtx(QGraphicsItem * parent=0);
    DragVtx(int x,int y,QGraphicsItem * parent=0);
    int coord[2];
    int range;
    QPen dotPen;
    QBrush rgBrush;
    QColor color;
    bool selected;

    virtual QRectF boundingRect() const Q_DECL_OVERRIDE;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;
    void setCoord(int x,int y);
};

class DragBBox : public QGraphicsItem
{
public:
    DragBBox(QGraphicsItem * paren=0);
    DragBBox(int l,int t,int r,int b,QGraphicsItem * parent=0);
    int bbid;
    unsigned char rgb[3];
    char txt[100];
    TrkScene* trkscene;
    QPen linepen;
    QBrush rectBrush;
    std::vector<DragVtx*> vtx;
    bool selected, isGood;

    int x1,x2,y1,y2;
    virtual QRectF boundingRect() const Q_DECL_OVERRIDE;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;
    void setClr(unsigned char r,unsigned char g,unsigned char b);
    void setCoord(int t,int l,int r,int b);
    void focused();
    void unfocused();
    void clone(DragBBox*& abox);
    void setVtx(int l,int t,int r,int b);
    void getltrb(int &l, int &t, int &r, int &b);
/*
signals:
    void clicked(int idx);
*/
};

class DragDots : public QGraphicsItem
{
public:
    DragDots(QGraphicsItem * paren=0);
    DragDots(int x,int y,QGraphicsItem * parent=0);
    int coord[2];
    int pid;
    unsigned char rgb[3];
    char txt[100];
    TrkScene* trkscene;
    QPen linepen;
    QBrush rectBrush;
    QFont txtfont;
    QRectF* recttmp;
    bool selected, isGood,ismovable;
    int range,x1,y1,baseRange;
    virtual QRectF boundingRect() const Q_DECL_OVERRIDE;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;

    void setClr(unsigned char r,unsigned char g,unsigned char b);
    void setCoord(int x,int y);
    void focused();
    void unfocused();
    void lit();
    void unlit();
/*
signals:
    void clicked(int idx);
    */
};
class Anchor : public QGraphicsItem
{
public:
    Anchor(int x,int y,QGraphicsItem * parent=0);
    Anchor(QGraphicsItem * parent=0);
    int coord[2];
    int pid;
    unsigned char rgb[3];
    char txt[100];
    QPen linepen;
    QBrush rectBrush;
    QFont txtfont;
    int range;
    int x1,y1;
    int selected;
    virtual QRectF boundingRect() const Q_DECL_OVERRIDE;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
    void setCoord(int x,int y);
    void setX(int x);
    void setClr(unsigned char r,unsigned char g,unsigned char b);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;
};
#endif // MODELSQT
