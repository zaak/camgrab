#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "console.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->consoleDockWidget->setVisible(false);

    Console::setOutputControl(ui->consoleOutput);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startCamera(bool a)
{
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    qDebug() << cameras.size();
    foreach (const QCameraInfo &cameraInfo, cameras) {

        qDebug() << cameraInfo.deviceName();
        qDebug() << cameraInfo.description();

        Console::log(cameraInfo.deviceName());
//        if (cameraInfo.deviceName() == "mycamera")
        camera = new QCamera(QCameraInfo::defaultCamera());

        camera->setViewfinder(ui->cameraViewFinder);
        camera->start();

        ui->statusBar->showMessage("Aaaaa");
    }
}

