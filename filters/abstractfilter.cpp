#include "abstractfilter.h"

AbstractFilter::AbstractFilter(QObject *parent):
    QObject(parent),
    enabled(false)
{}

bool AbstractFilter::isEnabled()
{
    return enabled;
}

void AbstractFilter::setEnabled(bool enable)
{
    enabled = enable;
}

const QString &AbstractFilter::getName()
{
    return name;
}
