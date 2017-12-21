#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "console.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Console::setOutputControl(ui->consoleOutput);

    QWidget *horizontalSpacer = new QWidget();
    horizontalSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->mainToolBar->addWidget(horizontalSpacer);

    ui->mainToolBar->addWidget(new QComboBox());

    ui->consoleDockWidget->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startCamera(bool enable)
{
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();

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
