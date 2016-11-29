#include "analogsensor.h"


AnalogSensor::AnalogSensor(uint8_t deviceAddr, uint8_t adcIndex)
    : AbstractDevice()
    , addr(deviceAddr)
{
    dataLen = 4;//pack - crc
    CommData* data = new CommData();
    data->autoUpdate=false;
    data->autoUpdatePeriod=10000;
    data->autoWrite=false;
    data->defaultValue=0;
    data->readCode = 0;
    data->writeCode = 0;
    data->checkAfterWrite = false;
    data->changed=false;
    params[(int)AbstractDevice::CurValue] = data;
    paramIter=params;
    setIndex(adcIndex);
}

QByteArray AnalogSensor::prefix()
{
    char buf[] = {(char)addr};
    return QByteArray(buf,1);
}

QByteArray AnalogSensor::stripPrefix(const QByteArray &data)
{
    if(data.isEmpty())
        return data;
    else
        return data.right(data.length()-2);
}

bool AnalogSensor::isValid()
{
    return (addr != 0);
}

uint16_t AnalogSensor::nativeValue()
{
    const uint16_t val = params[(int)AbstractDevice::CurValue]->value();
    params[(int)AbstractDevice::CurValue]->commitUpdated();
    return val;
}

bool AnalogSensor::isUpdated()
{
    return params[(int)AbstractDevice::CurValue]->isUpdated();
}

AbstractDevice::DeviceType AnalogSensor::deviceType()
{
    return AbstractDevice::DeviceType::DeviceType_AnalogSensor;
}

void AnalogSensor::setIndex(uint8_t index)
{
    if(index<10)
    {
        params[(int)AbstractDevice::CurValue]->readCode=10+index;
        params[(int)AbstractDevice::CurValue]->autoUpdate=true;
    }
}
