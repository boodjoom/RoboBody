#ifndef PWMDRIVE_H
#define PWMDRIVE_H
#include "drive.h"
#include "RS485.h"

class PwmDrive : public Drive
{
protected:
    enum PwmDriveParams
    {
        RefSpeed=AbstractDevice::RefValue,
        CurSpeed=AbstractDevice::CurValue
    };
public:
    PwmDrive(uint8_t driveAddr);
    virtual AbstractDevice::DeviceType deviceType();
};

#endif // PWMDRIVE_H
