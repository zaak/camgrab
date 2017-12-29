#ifndef ABSTRACTFILTER_H
#define ABSTRACTFILTER_H

#include <QObject>
#include <opencv2/core/core.hpp>

class AbstractFilter : public QObject
{
    Q_OBJECT
public:
    explicit AbstractFilter(QObject *parent = nullptr);
    virtual void apply(cv::Mat &mat) = 0;

signals:

public slots:
};

#endif // ABSTRACTFILTER_H
