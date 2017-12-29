#ifndef GLVIDEOWIDGET_H
#define GLVIDEOWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QDebug>
#include <QOpenGLTexture>
#include "glvideosurface.h"
#include "filters/facedetectfilter.h"

class GLVideoWidget : public QOpenGLWidget
{
    Q_OBJECT
private:
    GLVideoSurface *videoSurface;

public:
    explicit GLVideoWidget(QWidget *parent = nullptr);
    GLVideoSurface *getVideoSurface();
    QImage &getRenderedImage();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    void renderImage(const QImage &image);

private:
    QImage renderedImage;
    QPoint renderPosition;
    QSize renderSize;
    cv::Mat opencvFrame;
    QList<QSharedPointer<AbstractFilter>> filters;

    void calculateRenderPosition();

private slots:
    void cleanup();

public slots:
    void showImage(const QImage& image);
    void renderFrame(cv::Mat &frame);
};

#endif // GLVIDEOWIDGET_H
