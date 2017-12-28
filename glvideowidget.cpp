#include "glvideowidget.h"

using namespace std;
using namespace cv;


GLVideoWidget::GLVideoWidget(QWidget *parent):
    QOpenGLWidget(parent),
    videoSurface(new GLVideoSurface(this))
{

    connect(videoSurface, SIGNAL(frameReceived(cv::Mat&)), this, SLOT(renderFrame(cv::Mat&)));
    connect(videoSurface, SIGNAL(presentationStopped()), this, SLOT(cleanup()));

    face_cascade_gpu = cv::cuda::CascadeClassifier::create("/media/zaak/Data/dev/CamGrab/opencv/data/haarcascades_cuda/haarcascade_frontalface_alt2.xml");

    //qDebug() << "Haar loaded: " << face_cascade_gpu;
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
    //cv::cvtColor(mat, opencvFrame, CV_BGR2RGBA);

    //Mat frame(opencvFrame);

    Mat frame(mat);
        std::vector<Rect> faces;
         Mat frame_gray;
         Mat crop;
         Mat res;
         Mat gray;
         string text;
         stringstream sstm;

         cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
         equalizeHist(frame_gray, frame_gray);

         cv::cuda::GpuMat frame_gpu(frame_gray);
         cv::cuda::GpuMat objbuf_gpu;

         // Detect faces
         face_cascade_gpu->detectMultiScale(frame_gpu, objbuf_gpu);
         face_cascade_gpu->convert(objbuf_gpu, faces);

         // Set Region of Interest
         cv::Rect roi_b;
         cv::Rect roi_c;

         size_t ic = 0; // ic is index of current element
         int ac = 0; // ac is area of current element

         size_t ib = 0; // ib is index of biggest element
         int ab = 0; // ab is area of biggest element

         for (ic = 0; ic < faces.size(); ic++) // Iterate through all current elements (detected faces)

         {
             roi_c.x = faces[ic].x;
             roi_c.y = faces[ic].y;
             roi_c.width = (faces[ic].width);
             roi_c.height = (faces[ic].height);

             ac = roi_c.width * roi_c.height; // Get the area of current element (detected face)

             roi_b.x = faces[ib].x;
             roi_b.y = faces[ib].y;
             roi_b.width = (faces[ib].width);
             roi_b.height = (faces[ib].height);

             ab = roi_b.width * roi_b.height; // Get the area of biggest element, at beginning it is same as "current" element

             if (ac > ab)
             {
                 ib = ic;
                 roi_b.x = faces[ib].x;
                 roi_b.y = faces[ib].y;
                 roi_b.width = (faces[ib].width);
                 roi_b.height = (faces[ib].height);
             }

             crop = frame(roi_b);
             cv::resize(crop, res, Size(128, 128), 0, 0, INTER_LINEAR); // This will be needed later while saving images
             cvtColor(crop, gray, CV_BGR2GRAY); // Convert cropped image to Grayscale

             Point pt1(faces[ic].x, faces[ic].y); // Display detected faces on main window - live stream from camera
             Point pt2((faces[ic].x + faces[ic].height), (faces[ic].y + faces[ic].width));
             cv::rectangle(frame, pt1, pt2, Scalar(0, 255, 0), 2, 8, 0);
         }

         // Show image

         //putText(frame, text, cvPoint(30, 30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 0, 255), 1, CV_AA);


    cv::cvtColor(frame, opencvFrame, CV_BGR2RGBA);
    showImage(QImage(opencvFrame.data, opencvFrame.cols, opencvFrame.rows, QImage::Format_RGB32));
//    showImage(QImage(frame.data, frame.cols, frame.rows, QImage::Format_RGB32));
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
