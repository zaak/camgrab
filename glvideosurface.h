#ifndef GLVIDEOSURFACE_H
#define GLVIDEOSURFACE_H

#include <QAbstractVideoSurface>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

class GLVideoSurface: public QAbstractVideoSurface
{
    Q_OBJECT

public:
    explicit GLVideoSurface(QObject *parent = nullptr);

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;
    bool present(const QVideoFrame &frame);
    bool start(const QVideoSurfaceFormat &format);
    void stop();

signals:
    void frameReceived(cv::Mat &mat);
    void presentationStarted();
    void presentationStopped();
};

#endif // GLVIDEOSURFACE_H
