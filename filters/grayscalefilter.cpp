#include "grayscalefilter.h"

GrayscaleFilter::GrayscaleFilter(QObject *parent): AbstractFilter(parent)
{
    name = "Grayscale";
}

void GrayscaleFilter::apply(cv::Mat &mat)
{
    cv::Mat grayed;

    cv::cvtColor(mat, grayed, cv::COLOR_BGR2GRAY);
    cv::cvtColor(grayed, mat, cv::COLOR_GRAY2BGR);
}
