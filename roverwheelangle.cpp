#include "roverwheelangle.h"
#include <QSettings>

RoverWheelAngle::RoverWheelAngle(QSettings &settings)
    :AngleDrive(0)
{
    bool ok;
    addr = settings.value("DeviceAddress",0).toUInt(&ok);
    if(!ok)
        qCritical()<<"DeviceAddress: invalid value";
    _leftNativeAngle = settings.value("LeftNativeAngle",1000).toUInt(&ok);
    if(!ok)
        qCritical()<<"LeftNativeAngle: invalid value";
    _centerNativeAngle = settings.value("CenterNativeAngle",2000).toUInt(&ok);
    if(!ok)
        qCritical()<<"CenterNativeAngle: invalid value";
    _rightNativeAngle = settings.value("RightNativeAngle",3000).toUInt(&ok);
    if(!ok)
        qCritical()<<"RightNativeAngle: invalid value";
    _currentNativeAngle=7777;
     qInfo()<<"RoverWheelAngle "<<addr<<" with L:"<<_leftNativeAngle<<" C:"<<_centerNativeAngle<<" R:"<<_rightNativeAngle<<" added";
}

void RoverWheelAngle::setRefAngle(double angle)
{
    if(angle > 0.05)        //right
        _currentNativeAngle=_rightNativeAngle;
    else
        if(angle < -0.05)   //left
            _currentNativeAngle=_leftNativeAngle;
        else                //center
            _currentNativeAngle=_centerNativeAngle;

    params[(int)RefAngle]->setValue(_currentNativeAngle);
}

uint16_t RoverWheelAngle::getNativeRefAngle()
{
    return _currentNativeAngle;
}
