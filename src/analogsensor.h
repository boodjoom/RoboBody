#ifndef ANALOGSENSOR_H
#define ANALOGSENSOR_H
#include "abstractdevice.h"

class AnalogSensor: public AbstractDevice
{
public:
    AnalogSensor(uint8_t deviceAddr, uint8_t adcIndex);
    uint8_t addr;
    uint8_t index;
    virtual QByteArray prefix();
    virtual QByteArray stripPrefix(const QByteArray& data);
    virtual bool isValid();
    uint16_t nativeValue();
    bool isUpdated();
    virtual AbstractDevice::DeviceType deviceType();
};

#endif // ANALOGSENSOR_H
