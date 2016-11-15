#ifndef ROVERWHEELANGLE_H
#define ROVERWHEELANGLE_H
#include "angledrive.h"

class QSettings;

class RoverWheelAngle : public AngleDrive
{
public:
    RoverWheelAngle(QSettings &settings);
    void setRefAngle(double angle);
    uint16_t getNativeRefAngle();
protected:
    uint16_t _leftNativeAngle;
    uint16_t _centerNativeAngle;
    uint16_t _rightNativeAngle;
    uint16_t _currentNativeAngle;
};

#endif // ROVERWHEELANGLE_H
