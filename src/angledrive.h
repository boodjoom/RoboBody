#ifndef ANGLEDRIVE_H
#define ANGLEDRIVE_H
#include "drive.h"
#include "RS485.h"

class AngleDrive : public Drive
{
protected:
    enum AngleDriveParams
    {
        RefAngle=AbstractDevice::RefValue,
        CurAngle=AbstractDevice::CurValue
    };
public:
    AngleDrive(uint8_t addr);
    virtual AbstractDevice::DeviceType deviceType();
};

#endif // ANGLEDRIVE_H
