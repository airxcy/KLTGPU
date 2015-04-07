

#include "Qts/mainwindow.h"

#include <QApplication>
#include <iostream>

//#include <opencv2/opencv.hpp>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);

    QApplication app(argc, argv);
    MainWindow mainwindow;
    mainwindow.show();
    return app.exec();
}
