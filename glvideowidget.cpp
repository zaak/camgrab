#include "glvideowidget.h"
#include <QDebug>
#include <QOpenGLTexture>

GLVideoWidget::GLVideoWidget(QWidget *parent):
    QOpenGLWidget(parent),
    videoSurface(new GLVideoSurface(this))
{

    connect(videoSurface, SIGNAL(frameReceived(QVideoFrame)), this, SLOT(renderFrame(QVideoFrame)));
    connect(videoSurface, SIGNAL(presentationStopped()), this, SLOT(cleanup()));
}

GLVideoSurface *GLVideoWidget::getVideoSurface()
{
    return videoSurface;
}

void GLVideoWidget::initializeGL()
{
    QOpenGLWidget::initializeGL();

    glClearColor(.3f, .3f, .3f, 1.0f);
}

void GLVideoWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, -height, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    calculateRenderPosition();

    update();
}

void GLVideoWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!renderedImage.isNull())
    {
        QImage scaledImage = renderedImage.scaled(renderSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        renderImage(scaledImage);
    }
}

void GLVideoWidget::renderImage(const QImage &image)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glPushMatrix();
    {
        glRasterPos2i(renderPosition.x(), renderPosition.y());
        glPixelZoom(1, -1);
        glDrawPixels(image.width(), image.height(), GL_BGRA, GL_UNSIGNED_BYTE, image.bits());
    }
    glPopMatrix();

    glFlush();
}

void GLVideoWidget::calculateRenderPosition()
{
    float dimensionRatio = (float)renderedImage.width()/(float)renderedImage.height();

    renderSize.setWidth(width());
    renderSize.setHeight(floor(width() / dimensionRatio));

    if (renderSize.height() > height())
    {
        renderSize.setHeight(height());
        renderSize.setWidth(floor(height() * dimensionRatio));
    }

    renderPosition.setX(floor((width() - renderSize.width()) / 2));
    renderPosition.setY(-floor((height() - renderSize.height()) / 2));
}

void GLVideoWidget::showImage(const QImage& image)
{
    renderedImage = image;
    calculateRenderPosition();
    update();
}

void GLVideoWidget::renderFrame(const QVideoFrame& buffer)
{
    QImage frameImage;
    QVideoFrame frame(buffer);
    frame.map(QAbstractVideoBuffer::ReadOnly);
    QImage::Format frameImageFormat = QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat());

    if (frameImageFormat != QImage::Format_Invalid) {
        showImage(QImage(frame.bits(), frame.width(), frame.height(), frameImageFormat));
        frame.unmap();
    }
}

void GLVideoWidget::cleanup()
{
    renderedImage = QImage();
    update();
}

QImage &GLVideoWidget::getRenderedImage()
{
    return renderedImage;
}
