#ifndef GLVIDEOWIDGET_H
#define GLVIDEOWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include "glvideosurface.h"

class GLVideoWidget : public QOpenGLWidget
{
    Q_OBJECT
private:
    GLVideoSurface *glVideoSurface;
public:
    explicit GLVideoWidget(QWidget *parent = nullptr);
    GLVideoSurface *videoSurface();
signals:
    void imageSizeChanged( int outW, int outH ); /// Used to resize the image outside the widget

public slots:
    bool showImage(const QImage& image); /// Used to set the image to be viewed
    bool renderFrame(const QVideoFrame& frame);

protected:
    void initializeGL(); /// OpenGL initialization
    void paintGL(); /// OpenGL Rendering
    void resizeGL(int width, int height);        /// Widget Resize Event

    void updateScene();
    void renderImage();

private:

    QImage mRenderQtImg;           /// Qt image to be rendered
    QImage mResizedImg;

    QColor mBgColor;		/// Background color

    float mImgRatio;             /// height/width ratio

    int mRenderWidth;
    int mRenderHeight;
    int mRenderPosX;
    int mRenderPosY;

    QOpenGLShader *vShader;
    QOpenGLShader *fShader;
    QOpenGLShaderProgram *shaderProgram;

    void recalculatePosition();
};

#endif // GLVIDEOWIDGET_H
