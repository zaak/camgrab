#ifndef FACEDETECTFILTER_H
#define FACEDETECTFILTER_H

#include "abstractfilter.h"
#include <opencv2/cudaobjdetect.hpp>
#include <opencv2/cudaimgproc.hpp>

class FaceDetectFilter : public AbstractFilter
{
public:
    explicit FaceDetectFilter(QObject *parent = nullptr);
    void apply(cv::Mat &mat);
private:

    cv::Ptr<cv::cuda::CascadeClassifier> faceCascadeFilterGPU;
};

#endif // FACEDETECTFILTER_H
