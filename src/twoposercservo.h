#ifndef TWOPOSERCSERVO_H
#define TWOPOSERCSERVO_H
#include "rc_servo.h"
#include "gripperstate.h"

class QSettings;

class TwoPoseRCServo: public rc_servo
{
public:
    TwoPoseRCServo(QSettings &settings);
    void open();
    void close();
    GripperState getState();
protected:
    uint16_t _openedNativeValue;
    uint16_t _closedNativeValue;
    uint64_t _travelStartTimeMs;
    GripperState _nextState;
    GripperState _currentState;
};

#endif // TWOPOSERCSERVO_H
