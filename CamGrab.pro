QT += core gui multimedia multimediawidgets
QT += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CamGrab
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += /usr/local/include/

# pkg-config --libs opencv
LIBS += -L/usr/local/lib -lopencv_cudabgsegm -lopencv_cudaobjdetect -lopencv_cudastereo -lopencv_dnn -lopencv_ml -lopencv_shape -lopencv_stitching -lopencv_cudafeatures2d -lopencv_superres -lopencv_cudacodec -lopencv_videostab -lopencv_cudaoptflow -lopencv_cudalegacy -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_photo -lopencv_imgcodecs -lopencv_cudawarping -lopencv_cudaimgproc -lopencv_cudafilters -lopencv_video -lopencv_objdetect -lopencv_imgproc -lopencv_flann -lopencv_cudaarithm -lopencv_core -lopencv_cudev

QMAKE_RPATHDIR += $$PWD/opencv/build/lib/

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    console.cpp \
    cameramanager.cpp \
    dataawarecombobox.cpp \
    glvideosurface.cpp \
    glvideowidget.cpp \
    filters/abstractfilter.cpp \
    filters/facedetectfilter.cpp

HEADERS += \
    mainwindow.h \
    console.h \
    cameramanager.h \
    dataawarecombobox.h \
    glvideosurface.h \
    glvideowidget.h \
    filters/abstractfilter.h \
    filters/facedetectfilter.h

FORMS += \
    mainwindow.ui

DISTFILES +=

RESOURCES += \
    resources.qrc
