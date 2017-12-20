#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startCamera(bool a)
{
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach (const QCameraInfo &cameraInfo, cameras) {

        qDebug() << cameraInfo.deviceName();
        qDebug() << cameraInfo.description();
//        if (cameraInfo.deviceName() == "mycamera")
        camera = new QCamera(QCameraInfo::defaultCamera());

        camera->setViewfinder(ui->cameraViewFinder);
        camera->start();
    }
}

