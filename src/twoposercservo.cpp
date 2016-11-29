#include "twoposercservo.h"
#include <QSettings>
#include <QDateTime>
#include <QDebug>

TwoPoseRCServo::TwoPoseRCServo(QSettings &settings)
    :rc_servo(0,0)
{
    bool ok;

    addr = settings.value("DeviceAddress",0).toUInt(&ok);
    if(!ok)
        qCritical()<<"DeviceAddress: invalid value";

    number = settings.value("ServoIndex",5).toUInt(&ok);
    if(!ok)
        qCritical()<<"ServoIndex: invalid value";

    _openedNativeValue = settings.value("OpenedNativeValue",1000).toUInt(&ok);
    if(!ok)
        qCritical()<<"OpenedNativeValue: invalid value";

    _closedNativeValue = settings.value("ClosedNativeValue",2000).toUInt(&ok);
    if(!ok)
        qCritical()<<"ClosedNativeValue: invalid value";

    travelTime = settings.value("TravelTimeMs",2000).toUInt(&ok);
    if(!ok)
        qCritical()<<"TravelTime: invalid value";
    _travelStartTimeMs=0;
    _nextState = GripperState::Unknown;
    _currentState  = GripperState::Unknown;
     qInfo()<<"TwoPoseRCServo "<<addr<<":"<<number<<" with O:"<<_openedNativeValue
           <<" C:"<<_closedNativeValue<<" TT:"<<travelTime
          <<" added";
}

void TwoPoseRCServo::open()
{
    setRefAngle(_openedNativeValue);
    _travelStartTimeMs=QDateTime::currentMSecsSinceEpoch();
    _nextState = GripperState::Opened;
    _currentState = GripperState::Moving;
}

void TwoPoseRCServo::close()
{
    setRefAngle(_closedNativeValue);
    _travelStartTimeMs=QDateTime::currentMSecsSinceEpoch();
    _nextState = GripperState::Closed;
    _currentState = GripperState::Moving;
}

GripperState TwoPoseRCServo::getState()
{
    if(_nextState != _currentState)
    {
        if(QDateTime::currentMSecsSinceEpoch() >= _travelStartTimeMs+travelTime)
        {
            _currentState=_nextState;
        }
    }
    return _currentState;
}
