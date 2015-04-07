

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QGraphicsView>
#include <QLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QLabel>
#include <QSlider>

class StreamThread;
class TrkScene;
class GraphicsView;
class DefaultScene;
//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    StreamThread* streamThd;
    QWidget* cWidget;
    TrkScene* trkscene;
    GraphicsView* gview;
    DefaultScene* defaultscene;
    void setupLayout();
    void makeConns();
public slots:
    void gviewClicked(QGraphicsSceneMouseEvent * event);
    void initUI();
};
//! [0]

#endif // MAINWINDOW_H
