#include "rc_servo.h"
#include <QDebug>

void rc_servo::setRefAngle(uint16_t refAngle)
{
    params[(int)RefAngle]->setValue(refAngle);
}

rc_servo::rc_servo(uint8_t deviceAddr, uint8_t servoNum)
:AbstractDevice(),
  addr(deviceAddr)
, number(servoNum)
, travelTime(0)
{
    responseDataLen = 7;//pack - crc
    CommData* data = new CommData();
    data->autoUpdate=false;
    data->autoWrite=false;
    data->defaultValue=2000;
    data->readCode = 0;
    data->writeCode = 0;
    data->checkAfterWrite = false;
    data->changed=false;
    params[(int)RefAngle] = data;
    paramIter=params;
}

QByteArray rc_servo::prefix()
{
    char buf[] = {(char)0x7f,(char)addr,(char)number};
    return QByteArray(buf,3);
}

QByteArray rc_servo::suffix()
{
    char buf[] = {(char)(travelTime>>8),(char)(travelTime & (uint16_t)0x00FF)};
    return QByteArray(buf,2);
}

QByteArray rc_servo::stripPrefix(const QByteArray &data)
{
    if(data.isEmpty())
        return data;
    else
        return data.right(data.length()-3);
}

bool rc_servo::isValid()
{
    return (addr != 0);
}

AbstractDevice::DeviceType rc_servo::deviceType()
{
    return AbstractDevice::DeviceType::DeviceType_RCServo;
}
