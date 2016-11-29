#ifndef ANALOGSENSOR_H
#define ANALOGSENSOR_H
#include "abstractdevice.h"

class AnalogSensor: public AbstractDevice
{
public:
    AnalogSensor(uint8_t deviceAddr, uint8_t adcIndex = 0xFF);
    uint8_t addr;
    virtual QByteArray prefix();
    virtual QByteArray stripPrefix(const QByteArray& data);
    virtual bool isValid();
    uint16_t nativeValue();
    bool isUpdated();
    virtual AbstractDevice::DeviceType deviceType();
    void setIndex(uint8_t index);
};

#endif // ANALOGSENSOR_H
