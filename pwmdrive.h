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
    PwmDrive(uint8_t driveAddr);
};

#endif // PWMDRIVE_H
