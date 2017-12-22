#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCamera>
#include <QCameraInfo>
#include <QCameraViewfinder>
#include <QComboBox>
#include <QSpacerItem>
#include <QVariant>
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

    void detectCameras();

private slots:
    void toggleCamera(bool enable);
    void onCameraChanged(const QSharedPointer<QCamera> &cameraPtr);
};

#endif // MAINWINDOW_H
