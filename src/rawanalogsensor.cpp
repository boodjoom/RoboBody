#include "rawanalogsensor.h"
#include <QSettings>
#include <QDebug>

RawAnalogSensor::RawAnalogSensor(QSettings &settings)
    :AnalogSensor(0)
{
     bool ok;
     addr = settings.value("DeviceAddress",0).toUInt(&ok);
     if(!ok)
         qCritical()<<"DeviceAddress: invalid value";

     uint8_t index = settings.value("SensorIndex",0xFF).toUInt(&ok);
     if(!ok)
     {
         qCritical()<<"SensorIndex: invalid value";
         index=0xFF;
     }
     setIndex(index);

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
    double val = 0.0;
    uint16_t nat = nativeValue();
    if(nat >= _minNativeValue && nat <= _maxNativeValue)
    {
        const double cur = nativeValue()-_minNativeValue;
        val=(cur/_distance);
    }
    return val;
}

bool RawAnalogSensor::isValueValid()
{
    uint16_t cur = params[(int)AbstractDevice::CurValue]->value();
    return (cur >= _minNativeValue && cur <= _maxNativeValue);
}
