#include "glvideowidget.h"
#include <QDebug>
#include <QOpenGLTexture>

GLVideoWidget::GLVideoWidget(QWidget *parent):
    QOpenGLWidget(parent),
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
    //QOpenGLWidget::initializeGL();

    //makeCurrent();
    //initializeOpenGLFunctions();

//    float r = ((float)mBgColor.darker().red())/255.0f;
//    float g = ((float)mBgColor.darker().green())/255.0f;
//    float b = ((float)mBgColor.darker().blue())/255.0f;
//    glClearColor(r,g,b,1.0f);


//    vShader = new QOpenGLShader(QOpenGLShader::Vertex);
//    qDebug() << "vShader " << vShader->compileSourceFile(":/resources/shaders/blur.vert");

    fShader = new QOpenGLShader(QOpenGLShader::Fragment);
    qDebug() << "fShader " << fShader->compileSourceFile(":/resources/shaders/blur.frag");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    shaderProgram = new QOpenGLShaderProgram(this);
 //   shaderProgram->addShader(vShader);
    shaderProgram->addShader(fShader);
    shaderProgram->link();
    shaderProgram->bind();
 //   shaderProgram->release();
}

void GLVideoWidget::resizeGL(int width, int height)
{
    makeCurrent();
    glViewport(0, 0, (GLint)width, (GLint)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, width, -height, 0, 0, 1);

    glMatrixMode(GL_MODELVIEW);

//    QSurfaceFormat format;
//    format.setDepthBufferSize(24);
//    format.setStencilBufferSize(8);
//    format.setVersion(3, 2);
//    format.setProfile(QSurfaceFormat::CoreProfile);
//    setFormat(format);

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

            QOpenGLTexture *texture = new QOpenGLTexture(mResizedImg);
            texture->bind(0);
            shaderProgram->setUniformValue("colorTexturem", 0);
            shaderProgram->setUniformValue("resolution", mResizedImg.size());
            shaderProgram->setUniformValue("renderX", mRenderPosX);
            shaderProgram->setUniformValue("renderY", mRenderPosY);

            glDrawPixels(mResizedImg.width(), mResizedImg.height(), GL_BGRA, GL_UNSIGNED_BYTE, mResizedImg.bits());

            delete texture;
        }
        glPopMatrix();

        // end
        glFlush();
    }
}

void GLVideoWidget::recalculatePosition()
{
    mImgRatio = (float)mRenderQtImg.width()/(float)mRenderQtImg.height();

    mRenderWidth = width();
    mRenderHeight = floor(mRenderWidth / mImgRatio);

    if (mRenderHeight > height())
    {
        mRenderHeight = height();
        mRenderWidth = floor(mRenderHeight * mImgRatio);
    }

    mRenderPosX = floor((width() - mRenderWidth) / 2);
    mRenderPosY = -floor((height() - mRenderHeight) / 2);

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

    return showImage(img);
}
