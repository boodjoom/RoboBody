#ifndef ROVERWHEELDRIVE_H
#define ROVERWHEELDRIVE_H

#include "pwmdrive.h"

class QSettings;

class RoverWheelDrive : public PwmDrive
{
public:
    RoverWheelDrive(QSettings& settings);
    void setRefSpeed(double speed);
protected:
    uint16_t _fwdNativeRefSpeed;
    uint16_t _bkwNativeRefSpeed;
};

#endif // ROVERWHEELDRIVE_H
