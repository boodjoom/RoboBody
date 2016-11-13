#ifndef PWMDRIVE_H
#define PWMDRIVE_H
#include "drive.h"
#include "RS485.h"

class PwmDrive : public Drive
{
    enum PwmDriveParams
    {
        RefSpeed=AbstractDevice::RefValue,
        CurSpeed=AbstractDevice::CurValue
    };
public:
    PwmDrive(uint8_t driveAddr);
};

#endif // PWMDRIVE_H
