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
    bool isEnabled();
    const QString &getName();

protected:
    QString name;
    bool enabled;

public slots:
    void setEnabled(bool enable);
};

#endif // ABSTRACTFILTER_H
