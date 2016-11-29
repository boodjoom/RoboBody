#include "rawanalogsensor.h"
#include <QSettings>
#include <QDebug>

RawAnalogSensor::RawAnalogSensor(QSettings &settings)
    :AnalogSensor(0,0)
{
     bool ok;
     addr = settings.value("DeviceAddress",0).toUInt(&ok);
     if(!ok)
         qCritical()<<"DeviceAddress: invalid value";

     index = settings.value("SensorIndex",9).toUInt(&ok);
     if(!ok)
         qCritical()<<"SensorIndex: invalid value";

     _minNativeValue = settings.value("MinNativeValue",0).toUInt(&ok);
     if(!ok)
         qCritical()<<"MinNativeValue: invalid value";

     _maxNativeValue = settings.value("MaxNativeValue",0).toUInt(&ok);
     if(!ok)
         qCritical()<<"MinNativeValue: invalid value";

     _distance = _maxNativeValue - _minNativeValue;
}

double RawAnalogSensor::value()
{
    const double cur = nativeValue()-_minNativeValue;
    return (cur/_distance);
}

bool RawAnalogSensor::isValueValid()
{
    uint16_t cur = params[(int)AbstractDevice::CurValue]->value();
    return (cur >= _minNativeValue && cur <= _maxNativeValue);
}
