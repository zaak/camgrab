#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "console.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    cameraManager(new CameraManager)
{
    ui->setupUi(this);
    Console::setOutputControl(ui->consoleOutput);

    QWidget *horizontalSpacer = new QWidget(ui->mainToolBar);
    horizontalSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->mainToolBar->addWidget(horizontalSpacer);

    cameraComboBox = new DataAwareComboBox(ui->mainToolBar);
    cameraComboBoxAction = ui->mainToolBar->addWidget(cameraComboBox);

    connect(cameraComboBox, SIGNAL(activated(QVariant)), cameraManager, SLOT(changeSelectedCamera(QVariant)));
    connect(cameraManager, SIGNAL(changedSelectedCamera(QSharedPointer<QCamera>)), this, SLOT(onCameraChanged(QSharedPointer<QCamera>)));

    //ui->consoleDockWidget->setVisible(false);

    detectCameras();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::detectCameras()
{
    QList<QCameraInfo> cameras = cameraManager->listCameras();

    if (cameras.empty()) {
        Console::error("No available cameras detected!");
        cameraComboBoxAction->setVisible(false);
        ui->consoleDockWidget->setVisible(true);
        return;
    }

    Console::log(QString("Detected %1 cameras:").arg(cameras.size()));

    foreach (const QCameraInfo &cameraInfo, cameras) {
        Console::log(cameraInfo.description());
        cameraComboBox->addItem(cameraInfo.description(), QVariant::fromValue(cameraInfo));
    }

    cameraManager->changeSelectedCamera(cameras.first());
}

void MainWindow::onCameraChanged(const QSharedPointer<QCamera> &cameraPtr)
{
    ui->actionToggleCamera->setChecked(false);

    QCamera *camera = cameraPtr.data();
    camera->setViewfinder(ui->cameraViewFinder);
}

void MainWindow::toggleCamera(bool enable)
{
    QCamera *camera = cameraManager->getSelectedCamera().data();
    QCameraInfo cameraInfo = cameraManager->getSelectedCameraInfo();

    if (enable) {
        Console::log(QString("Starting camera %1").arg(cameraInfo.description()));
        camera->load();
        camera->start();

        qDebug() << "MainWindow::toggleCamera" << camera->supportedViewfinderResolutions();
        qDebug() << "MainWindow::toggleCamera" << camera->supportedViewfinderPixelFormats();
    } else {
        Console::log(QString("Stopping camera %1").arg(cameraInfo.description()));
        camera->stop();
    }
}
