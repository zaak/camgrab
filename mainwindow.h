#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCamera>
#include <QCameraInfo>
#include <QCameraViewfinder>
#include <QComboBox>
#include <QSettings>
#include <QSpacerItem>
#include <QTimer>
#include <QVariant>
#include <QVideoProbe>
#include "cameramanager.h"
#include "dataawarecombobox.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QCamera *camera;
    CameraManager *cameraManager;
    DataAwareComboBox *cameraComboBox;
    QAction *cameraComboBoxAction;
    QTimer *fpsTimer;
    unsigned int framesInCurrentSecond = 0;

    QString outputDirectoryPath;

    void detectCameras();
    void readSettings();
    void writeSettings();

private slots:
    void toggleCamera(bool enable);
    void processFrame(const QVideoFrame &frame);
    void onCameraChanged(const QSharedPointer<QCamera> &cameraPtr);
    void updateFps();
    void chooseOutputDirectory();
};

#endif // MAINWINDOW_H
