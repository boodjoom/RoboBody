#include "manipangle.h"
#include <QSettings>
#include <QDateTime>

ManipAngle::ManipAngle(QSettings &settings)
    :AngleDrive(0)
{
    bool ok;
    addr = settings.value("DeviceAddress",0).toUInt(&ok);
    if(!ok)
        qCritical()<<"DeviceAddress: invalid value";
    _targetNativeAngle = settings.value("TargetNativeAngle",400).toUInt(&ok);
    if(!ok)
        qCritical()<<"TargetNativeAngle: invalid value";
    _homeNativeAngle = settings.value("HomeNativeAngle",100).toUInt(&ok);
    if(!ok)
        qCritical()<<"HomeNativeAngle: invalid value";
    _travelTimeMs = settings.value("TravelTimeMs",2000).toUInt(&ok);
    if(!ok)
        qCritical()<<"TravelTime: invalid value";
    _useTravelTime = settings.value("UseTravelTime",true).toBool();
    if(!_useTravelTime)
    {
        qInfo()<<"Only UseTravelTime supported";
        _useTravelTime=true;
    }
    _travelStartTimeMs=0;
    _currentNativeAngle=7777;
    _nextState = ManipState::Unknown;
    _currentState  = ManipState::Unknown;
     qInfo()<<"ManipAngle "<<addr<<" with T:"<<_targetNativeAngle
           <<" H:"<<_homeNativeAngle<<" TT:"<<_travelTimeMs
          <<" UTT:"<<_useTravelTime<<" added";

}

void ManipAngle::moveToTarget()
{
    params[(int)RefAngle]->setValue(_targetNativeAngle);
    _travelStartTimeMs=QDateTime::currentMSecsSinceEpoch();
    _currentState = ManipState::Moving;
    _nextState = ManipState::AtTarget;
}

void ManipAngle::moveToHome()
{
    params[(int)RefAngle]->setValue(_homeNativeAngle);
    _travelStartTimeMs=QDateTime::currentMSecsSinceEpoch();
    _currentState = ManipState::Moving;
    _nextState = ManipState::AtHome;
}

void ManipAngle::moveToBase()
{
    params[(int)RefAngle]->setValue(0);
    _travelStartTimeMs=QDateTime::currentMSecsSinceEpoch();
    _currentState = ManipState::Moving;
    _nextState = ManipState::AtBase;
}

ManipState ManipAngle::getState()
{
    if(_nextState != _currentState)
        updateState();
    return _currentState;
}

void ManipAngle::updateState()
{
    if(_useTravelTime)
    {
        if(_travelStartTimeMs+_travelTimeMs < QDateTime::currentMSecsSinceEpoch())
        {
            _currentState = _nextState;
        }
    }
}
