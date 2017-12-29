#ifndef FACEDETECTFILTER_H
#define FACEDETECTFILTER_H

#include "abstractfilter.h"
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/cudaobjdetect.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudawarping.hpp>

class FaceDetectFilter : public AbstractFilter
{
public:
    explicit FaceDetectFilter(QObject *parent = nullptr);
    void apply(cv::Mat &mat);
private:

    cv::Ptr<cv::cuda::CascadeClassifier> face_cascade_gpu;
};

#endif // FACEDETECTFILTER_H
