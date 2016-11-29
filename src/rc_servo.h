#ifndef RC_SERVO_H
#define RC_SERVO_H
#include "abstractdevice.h"

class rc_servo: public AbstractDevice
{
protected:
    enum RCServoParams
    {
        RefAngle=AbstractDevice::RefValue
    };
public:
    void setRefAngle(uint16_t refAngle);
    rc_servo(uint8_t deviceAddr, uint8_t servoNum);
    uint8_t addr;
    uint8_t number;
    uint16_t travelTime;
    virtual QByteArray prefix();
    virtual QByteArray suffix();
    virtual QByteArray stripPrefix(const QByteArray& data);
    virtual bool isValid();
    virtual AbstractDevice::DeviceType deviceType();
};

#endif // RC_SERVO_H
