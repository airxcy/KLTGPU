#-------------------------------------------------
#
# Project created by QtCreator 2014-12-22T23:23:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KLTGPU
TEMPLATE = app


SOURCES += \
    src/Qts/main.cpp \
    src/Qts/mainwindow.cpp \
    src/Qts/streamthread.cpp \
    src/Qts/viewqt.cpp \
    src/trackers/buffers.cpp \
    src/Qts/modelsqt.cpp \
    src/trackers/utils.cpp \
    src/trackers/klt_c/convolve.cpp \
    src/trackers/klt_c/error.cpp \
    src/trackers/klt_c/klt.cpp \
    src/trackers/klt_c/klt_util.cpp \
    src/trackers/klt_c/pnmio.cpp \
    src/trackers/klt_c/pyramid.cpp \
    src/trackers/klt_c/selectGoodFeatures.cpp \
    src/trackers/klt_c/storeFeatures.cpp \
    src/trackers/klt_c/trackFeatures.cpp \
    src/trackers/klt_c/trk.cpp \
    src/trackers/klt_c/writeFeatures.cpp \
    src/trackers/klttracker.cpp

HEADERS  += \
    includes/Qts/mainwindow.h \
    includes/Qts/streamthread.h \
    includes/Qts/viewqt.h \
    includes/trackers/buffers.h \
    includes/Qts/modelsqt.h \
    includes/trackers/utils.h \
    includes/trackers/klt_c/base.h \
    includes/trackers/klt_c/convolve.h \
    includes/trackers/klt_c/error.h \
    includes/trackers/klt_c/klt.h \
    includes/trackers/klt_c/klt_util.h \
    includes/trackers/klt_c/pnmio.h \
    includes/trackers/klt_c/pyramid.h \
    includes/trackers/klt_c/stdafx.h \
    includes/trackers/klt_c/targetver.h \
    includes/trackers/klt_c/trk.h \
    includes/trackers/tracker.h \
    includes/trackers/klttracker.h

FORMS    += mainwindow.ui

RESOURCES += resources.qrc

# install
target.path = build/
INSTALLS += target

INCLUDEPATH += includes

win32 {

    ## Windows common build here

    !contains(QMAKE_TARGET.arch, x86_64) {
        message("x86 build")

        ## Windows x86 (32bit) specific build here

    } else {
        message("x86_64 build")

        ## Windows x64 (64bit) specific build here

    }
}

LIBS += -LC:/opencv/buildGPU/lib/Release/ -lopencv_calib3d249\
-lopencv_contrib249\
-lopencv_core249\
-lopencv_features2d249\
-lopencv_flann249\
-lopencv_gpu249\
-lopencv_highgui249\
-lopencv_imgproc249\
-lopencv_legacy249\
-lopencv_ml249\
-lopencv_nonfree249\
-lopencv_objdetect249\
-lopencv_ocl249\
-lopencv_photo249\
-lopencv_stitching249\
-lopencv_superres249\
-lopencv_ts249\
-lopencv_video249\
-lopencv_videostab249

INCLUDEPATH += C:/opencv/build/include
DEPENDPATH += C:/opencv/build/include



