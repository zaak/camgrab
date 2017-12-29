#include "facedetectfilter.h"
#include <QApplication>

FaceDetectFilter::FaceDetectFilter(QObject *parent): AbstractFilter(parent)
{
    name = "Face detect";

    QString faceCascadePath = QApplication::applicationDirPath() + "/haarcascades_cuda/haarcascade_frontalface_alt2.xml";
    faceCascadeFilterGPU = cv::cuda::CascadeClassifier::create(faceCascadePath.toStdString());
}

void FaceDetectFilter::apply(cv::Mat &mat)
{
    std::vector<cv::Rect> faces;
    cv::Mat grayedFrame;

    cv::cvtColor(mat, grayedFrame, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(grayedFrame, grayedFrame);

    cv::cuda::GpuMat frameGPU(grayedFrame);
    cv::cuda::GpuMat objectBufferGPU;

    faceCascadeFilterGPU->detectMultiScale(frameGPU, objectBufferGPU);
    faceCascadeFilterGPU->convert(objectBufferGPU, faces);

    for (cv::Rect &face : faces)
    {
        cv::Point pt1(face.x, face.y);
        cv::Point pt2(face.x + face.height, face.y + face.width);
        cv::rectangle(mat, pt1, pt2, cv::Scalar(0, 255, 0), 2, 8, 0);
    }
}
