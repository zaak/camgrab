#ifndef GLVIDEOWIDGET_H
#define GLVIDEOWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QDebug>
#include <QOpenGLTexture>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/cudaobjdetect.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudawarping.hpp>

#include "glvideosurface.h"

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

    cv::Ptr<cv::cuda::CascadeClassifier> face_cascade_gpu;
    void calculateRenderPosition();

private slots:
    void cleanup();

public slots:
    void showImage(const QImage& image);
    void renderFrame(cv::Mat &mat);
};

#endif // GLVIDEOWIDGET_H
