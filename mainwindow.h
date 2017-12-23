#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCamera>
#include <QCameraInfo>
#include <QCameraViewfinder>
#include <QComboBox>
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
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QCamera *camera;
    CameraManager *cameraManager;
    DataAwareComboBox *cameraComboBox;
    QAction *cameraComboBoxAction;
    QTimer *fpsTimer;
    QVideoProbe *fpsProbe;

    void detectCameras();
    unsigned int framesInCurrentSecond = 0;

private slots:
    void toggleCamera(bool enable);
    void processFrame(const QVideoFrame &frame);
    void onCameraChanged(const QSharedPointer<QCamera> &cameraPtr);
    void updateFps();
};

#endif // MAINWINDOW_H
