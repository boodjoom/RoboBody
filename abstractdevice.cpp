#include "abstractdevice.h"

AbstractDevice::AbstractDevice():paramIter(params){}

bool AbstractDevice::hasNext()
{return paramIter.hasNext();}

QPair<int, CommData *> AbstractDevice::next(){paramIter.next();return QPair<int,CommData*>(paramIter.key(),paramIter.value());}

QPair<int, CommData *> AbstractDevice::current()
{
    return QPair<int,CommData*>(paramIter.key(),paramIter.value());
}

void AbstractDevice::toFront(){paramIter.toFront();}

AbstractDevice::DeviceType AbstractDevice::deviceType()
{
    return DeviceType_Unknown;
}
