#ifndef PWMDRIVE_H
#define PWMDRIVE_H
#include "drive.h"
#include "RS485.h"

class PwmDrive : public Drive
{
    enum PwmDriveParams
    {
        RefSpeed=1,
        CurSpeed=2
    };
public:
    PwmDrive(uint8_t driveAddr):Drive(driveAddr)
    {
        CommData* data = new CommData();
        data->autoUpdate=false;
        data->autoWrite=true;
        data->defaultValue=4095;
        data->readCode = READ_REFSPEED_FCN;
        data->writeCode = WRITE_REFSPEED_FCN;
        data->setValue(data->defaultValue);
        params[(int)RefSpeed] = data;
        data = new CommData();
        data->autoUpdate=true;
        data->autoWrite=false;
        data->defaultValue=4095;
        data->readCode = READ_REFSPEED_FCN;
        data->writeCode = 0;
        data->changed=false;
        params[(int)CurSpeed] = data;
    }
};

#endif // PWMDRIVE_H
