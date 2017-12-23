#-------------------------------------------------
#
# Project created by QtCreator 2017-12-20T17:14:44
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets
QT       += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CamGrab
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    console.cpp \
    cameramanager.cpp \
    dataawarecombobox.cpp \
    glvideosurface.cpp \
    glvideowidget.cpp

HEADERS += \
        mainwindow.h \
    console.h \
    cameramanager.h \
    dataawarecombobox.h \
    glvideosurface.h \
    glvideowidget.h

FORMS += \
        mainwindow.ui

DISTFILES +=

RESOURCES += \
    resources.qrc
