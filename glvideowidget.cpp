#include "glvideowidget.h"
#include <QDebug>

GLVideoWidget::GLVideoWidget(QWidget *parent):
    QGLWidget(parent),
    glVideoSurface(new GLVideoSurface(this))
{
    mBgColor = QColor::fromRgb(150, 150, 150);

    connect(glVideoSurface, SIGNAL(frameReceived(QVideoFrame)), this, SLOT(renderFrame(QVideoFrame)));
}

GLVideoSurface *GLVideoWidget::videoSurface()
{
    return glVideoSurface;
}

void GLVideoWidget::initializeGL()
{
    QGLWidget::initializeGL();

    makeCurrent();
    //initializeOpenGLFunctions();

    float r = ((float)mBgColor.darker().red())/255.0f;
    float g = ((float)mBgColor.darker().green())/255.0f;
    float b = ((float)mBgColor.darker().blue())/255.0f;
    glClearColor(r,g,b,1.0f);
}

void GLVideoWidget::resizeGL(int width, int height)
{
    makeCurrent();
    glViewport(0, 0, (GLint)width, (GLint)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, width, -height, 0, 0, 1);

    glMatrixMode(GL_MODELVIEW);

    recalculatePosition();

    emit imageSizeChanged(mRenderWidth, mRenderHeight);

    updateScene();
}

void GLVideoWidget::updateScene()
{
    if (this->isVisible()) update();
}

void GLVideoWidget::paintGL()
{
    makeCurrent();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderImage();
}

void GLVideoWidget::renderImage()
{
    makeCurrent();

    glClear(GL_COLOR_BUFFER_BIT);

    if (!mRenderQtImg.isNull())
    {
        glLoadIdentity();

        glPushMatrix();
        {
            if (mResizedImg.width() <= 0)
            {
                if (mRenderWidth == mRenderQtImg.width() && mRenderHeight == mRenderQtImg.height())
                    mResizedImg = mRenderQtImg;
                else
                    mResizedImg = mRenderQtImg.scaled(QSize(mRenderWidth, mRenderHeight),
                                                      Qt::IgnoreAspectRatio,
                                                      Qt::SmoothTransformation);
            }

            // ---> Centering image in draw area

            glRasterPos2i(mRenderPosX, mRenderPosY);

            glPixelZoom(1, -1);

            glDrawPixels(mResizedImg.width(), mResizedImg.height(), GL_RGBA, GL_UNSIGNED_BYTE, mResizedImg.bits());

        }
        glPopMatrix();

        // end
        glFlush();
    }
}

void GLVideoWidget::recalculatePosition()
{
    mImgRatio = (float)mRenderQtImg.width()/(float)mRenderQtImg.height();

    mRenderWidth = this->size().width();
    mRenderHeight = floor(mRenderWidth / mImgRatio);

    if (mRenderHeight > this->size().height())
    {
        mRenderHeight = this->size().height();
        mRenderWidth = floor(mRenderHeight * mImgRatio);
    }

    mRenderPosX = floor((this->size().width() - mRenderWidth) / 2);
    mRenderPosY = -floor((this->size().height() - mRenderHeight) / 2);

    mResizedImg = QImage();
}

bool GLVideoWidget::showImage(const QImage& image)
{
    mRenderQtImg = image;

    recalculatePosition();

    updateScene();

    return true;
}

bool GLVideoWidget::renderFrame(const QVideoFrame& buffer)
{
    QImage img;
    QVideoFrame frame(buffer);
    frame.map(QAbstractVideoBuffer::ReadOnly);
    QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat());

    if (imageFormat != QImage::Format_Invalid) {
        img = QImage(frame.bits(),
                     frame.width(),
                     frame.height(),
                     // frame.bytesPerLine(),
                     imageFormat);
    } else {
        int nbytes = frame.mappedBytes();
        img = QImage::fromData(frame.bits(), nbytes);
    }

    frame.unmap();


    qDebug() << img;

    return showImage(img);
}
