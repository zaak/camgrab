#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "console.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    cameraManager(new CameraManager(this)),
    fpsTimer(new QTimer(this))
{
    ui->setupUi(this);
    readSettings();
    showMaximized();
    Console::setOutputControl(ui->consoleOutput);

    QWidget *horizontalSpacer = new QWidget(ui->mainToolBar);
    horizontalSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->mainToolBar->addWidget(horizontalSpacer);

    cameraComboBox = new DataAwareComboBox(ui->mainToolBar);
    cameraComboBoxAction = ui->mainToolBar->addWidget(cameraComboBox);

    connect(cameraComboBox, SIGNAL(activated(QVariant)), cameraManager, SLOT(changeSelectedCamera(QVariant)));
    connect(cameraManager, SIGNAL(changedSelectedCamera(QSharedPointer<QCamera>)), this, SLOT(onCameraChanged(QSharedPointer<QCamera>)));

    connect(fpsTimer, SIGNAL(timeout()), this, SLOT(updateFps()));
    connect(ui->cameraViewFinder->videoSurface(), SIGNAL(frameReceived(QVideoFrame)), this, SLOT(processFrame(QVideoFrame)));

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

    if (cameras.size() == 1) {
        cameraComboBoxAction->setDisabled(true);
    }
}

void MainWindow::onCameraChanged(const QSharedPointer<QCamera> &cameraPtr)
{
    ui->actionToggleCamera->setChecked(false);

    QCamera *camera = cameraPtr.data();
    camera->setViewfinder(ui->cameraViewFinder->videoSurface());
}

void MainWindow::toggleCamera(bool enable)
{
    QCamera *camera = cameraManager->getSelectedCamera().data();
    QCameraInfo cameraInfo = cameraManager->getSelectedCameraInfo();

    if (enable) {
        Console::log(QString("Starting camera %1").arg(cameraInfo.description()));
        camera->load();
        camera->start();
        fpsTimer->start(1000);
    } else {
        Console::log(QString("Stopping camera %1").arg(cameraInfo.description()));
        camera->stop();
        fpsTimer->stop();
        ui->statusBar->clearMessage();
    }
}

void MainWindow::processFrame(const QVideoFrame &frame)
{
    ++framesInCurrentSecond;
}

void MainWindow::updateFps()
{
    ui->statusBar->showMessage(QString("%1 FPS").arg(framesInCurrentSecond));
    framesInCurrentSecond = 0;
}

void MainWindow::chooseOutputDirectory()
{
    QString outDirPath = QFileDialog::getExistingDirectory(this, tr("Output directory"), outputDirectoryPath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!outDirPath.isEmpty()) {
        outputDirectoryPath = outDirPath;
        ui->outDirEdit->setText(outputDirectoryPath);
        writeSettings();
        Console::log(QString("Changed output directory to %1").arg(outputDirectoryPath));
    }
}

void MainWindow::readSettings()
{
    QSettings settings;

    settings.beginGroup("output");
    outputDirectoryPath = settings.value("directory").toString();
    ui->outDirEdit->setText(outputDirectoryPath);
    settings.endGroup();
}

void MainWindow::writeSettings()
{
    QSettings settings;

    settings.beginGroup("output");
    settings.setValue("directory", outputDirectoryPath);
    settings.endGroup();
}
