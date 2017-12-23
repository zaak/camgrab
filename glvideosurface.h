#ifndef GLVIDEOSURFACE_H
#define GLVIDEOSURFACE_H

#include <QAbstractVideoSurface>

class GLVideoSurface: public QAbstractVideoSurface
{
    Q_OBJECT
public:
    explicit GLVideoSurface(QObject *parent = nullptr);

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
                QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;
    bool present(const QVideoFrame &frame);
signals:
    void frameReceived(const QVideoFrame& frame);

public slots:
};

#endif // GLVIDEOSURFACE_H
