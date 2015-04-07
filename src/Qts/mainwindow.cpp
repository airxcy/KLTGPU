#include "Qts/mainwindow.h"
#include "Qts/streamthread.h"
#include "Qts/viewqt.h"
#include "Qts/modelsqt.h"

//#include <direct.h>

#include <QtWidgets>
#include <QSizePolicy>
#include <iostream>
#include <QPalette>
#include <QKeySequence>
#include <QFontDatabase>
#include <QStringList>
//! [1]
char cbuff[200];
MainWindow::MainWindow()
{

    cWidget = new QWidget(this);
    setCentralWidget(cWidget);

    streamThd = new StreamThread(this);

    trkscene=NULL;
    setupLayout();
    makeConns();
    move(100, 0);

}
void MainWindow::setupLayout()
{
    /** views **/
    defaultscene = new DefaultScene(0, 0, 440, 240);
    gview = new GraphicsView(defaultscene,cWidget);
    gview->setFixedSize(defaultscene->width()+2,defaultscene->height()+2);
    gview->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    cWidget->setFixedSize(gview->minimumSize());
}
void MainWindow::makeConns()
{
    connect(defaultscene,SIGNAL(clicked(QGraphicsSceneMouseEvent *)),this,SLOT(gviewClicked(QGraphicsSceneMouseEvent *)));
    connect(streamThd,SIGNAL(initSig()),this,SLOT(initUI()),Qt::BlockingQueuedConnection);
}
void MainWindow::gviewClicked(QGraphicsSceneMouseEvent * event)
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open vid"), "/Users/xcy/Documents/CVProject/data/labelCross", tr("Vid Files (*.avi *.mp4 *.mkv *.mts)"));
    if(!fileName.isEmpty())
    {
        std::string tmpstr = fileName.toStdString();
        streamThd->streamStart(tmpstr);
    }
}

void MainWindow::initUI()
{
    int fw=streamThd->framewidth,fh=streamThd->frameheight;
    trkscene = new TrkScene(0, 0, fw, fh);
    trkscene->streamThd=streamThd;
    streamThd->trkscene=trkscene;
    gview->setFixedSize(fw+3,fh+3);
    gview->setScene(trkscene);
    trkscene->updateFptr(streamThd->frameptr,streamThd->frameidx);
    cWidget->setFixedSize(gview->minimumSize());
}
