#include "cameramanager.h"
#include "console.h"

CameraManager::CameraManager(QObject *parent)
    :QObject(parent)
{

}

QList<QCameraInfo> CameraManager::listCameras()
{
    return QCameraInfo::availableCameras();

    ////        if (cameraInfo.deviceName() == "mycamera")
    //        camera = new QCamera(QCameraInfo::defaultCamera());

    //        camera->setViewfinder(ui->cameraViewFinder);
    //        camera->start();

    //        ui->statusBar->showMessage("Aaaaa");
}

QSharedPointer<QCamera> CameraManager::getCameraByDeviceName(const QString &deviceName)
{
    QList<QCameraInfo> cameras = listCameras();

    foreach (const QCameraInfo &cameraInfo, cameras) {
        if (cameraInfo.deviceName() == deviceName) {
            return QSharedPointer<QCamera>(new QCamera(cameraInfo));
        }
    }

    return QSharedPointer<QCamera>(nullptr);
}

void CameraManager::changeSelectedCamera(const QVariant &variant)
{
    changeSelectedCamera(variant.toString());
}

void CameraManager::changeSelectedCamera(const QString &deviceName)
{
    QSharedPointer<QCamera> cameraPtr = getCameraByDeviceName(deviceName);

    if (cameraPtr.isNull()) {
        Console::error(QString("Couln't change selected camera to %1 (): null pointer returned").arg(deviceName));
    }

    selectedCamera = cameraPtr;
    selectedCameraInfo = QCameraInfo(*cameraPtr.data());

    emit changedSelectedCamera(cameraPtr);

    Console::log(QString("Changed selected camera to %1").arg(selectedCameraInfo.description()));
}

QSharedPointer<QCamera> CameraManager::getSelectedCamera()
{
    return selectedCamera;
}

QCameraInfo &CameraManager::getSelectedCameraInfo()
{
    return selectedCameraInfo;
}
