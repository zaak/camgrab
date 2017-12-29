#include "glvideowidget.h"

GLVideoWidget::GLVideoWidget(QWidget *parent):
    QOpenGLWidget(parent),
    videoSurface(new GLVideoSurface(this))
{
    connect(videoSurface, SIGNAL(frameReceived(cv::Mat&)), this, SLOT(renderFrame(cv::Mat&)));
    connect(videoSurface, SIGNAL(presentationStopped()), this, SLOT(cleanup()));

//    registerFilter(QSharedPointer<AbstractFilter>(new FaceDetectFilter()));
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

void GLVideoWidget::renderFrame(cv::Mat &mat)
{
    foreach (const QSharedPointer<AbstractFilter> &filterPtr, filters) {
        AbstractFilter *filter = filterPtr.data();
        if (filter->isEnabled()) {
            filter->apply(mat);
        }
    }

    cv::cvtColor(mat, opencvFrame, CV_BGR2RGBA);
    showImage(QImage(opencvFrame.data, opencvFrame.cols, opencvFrame.rows, QImage::Format_RGB32));
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

void GLVideoWidget::registerFilter(QSharedPointer<AbstractFilter> filter)
{
    filters.append(filter);
}

void GLVideoWidget::disableFilters()
{
    foreach (const QSharedPointer<AbstractFilter> &filterPtr, filters) {
        AbstractFilter *filter = filterPtr.data();
        if (filter->isEnabled()) {
            filter->setEnabled(false);
        }
    }
}
