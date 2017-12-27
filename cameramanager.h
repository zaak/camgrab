#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include <QObject>
#include <QList>
#include <QCameraInfo>
#include <QSharedPointer>

Q_DECLARE_METATYPE(QCameraInfo)

class CameraManager: public QObject
{
    Q_OBJECT
private:
    QSharedPointer<QCamera> selectedCamera;
    QCameraInfo selectedCameraInfo;

public:
    CameraManager(QObject *parent = nullptr);
    QList<QCameraInfo> listCameras();
    QSharedPointer<QCamera> getCameraByDeviceName(const QString &deviceName);
    QSharedPointer<QCamera> getSelectedCamera();
    QCameraInfo &getSelectedCameraInfo();

public slots:
    void changeSelectedCamera(const QVariant &variant);
    void changeSelectedCamera(const QCameraInfo &cameraInfo);

signals:
    void changedSelectedCamera(const QSharedPointer<QCamera> &);
};

#endif // CAMERAMANAGER_H
