#ifndef MANIPANGLE_H
#define MANIPANGLE_H
#include "angledrive.h"
#include "manipstate.h"

class QSettings;

class ManipAngle : public AngleDrive
{
public:
    ManipAngle(QSettings &settings);
    void moveToTarget();
    void moveToHome();
    void moveToBase();
    ManipState getState();
protected:
    uint16_t _calibNativeSpeed;
    uint16_t _travelNativeSpeed;
    uint16_t _targetNativeAngle;
    uint16_t _homeNativeAngle;
    uint16_t _travelTimeMs;
    uint16_t _currentNativeAngle;
    bool _useTravelTime;
    void updateState();
private:
    uint64_t _travelStartTimeMs;
    ManipState _nextState;
    ManipState _currentState;
};

#endif // MANIPANGLE_H
