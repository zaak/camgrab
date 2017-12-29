#ifndef GRAYSCALEFILTER_H
#define GRAYSCALEFILTER_H

#include <opencv2/imgproc.hpp>
#include "abstractfilter.h"

class GrayscaleFilter : public AbstractFilter
{
public:
    explicit GrayscaleFilter(QObject *parent = nullptr);
    void apply(cv::Mat &mat);
};

#endif // GRAYSCALEFILTER_H
