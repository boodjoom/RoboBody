#include "roverwheeldrive.h"
#include <QSettings>
#include <QDebug>

RoverWheelDrive::RoverWheelDrive(QSettings &settings)
    :PwmDrive(0)
{
    bool ok;
    addr = settings.value("DeviceAddress",0).toUInt(&ok);
    if(!ok)
        qCritical()<<"DeviceAddress: invalid value";
    _fwdNativeRefSpeed = settings.value("ForwardNativeSpeed",1000).toUInt(&ok);
    if(!ok)
        qCritical()<<"ForwardNativeSpeed: invalid value";
    _bkwNativeRefSpeed = settings.value("BackwardNativeSpeed",1000).toUInt(&ok);
    if(!ok)
        qCritical()<<"BackwardNativeSpeed: invalid value";
    qInfo()<<"RoverWheelDrive "<<addr<<" with F:"<<_fwdNativeRefSpeed<<" B:"<<_bkwNativeRefSpeed<<" added";
}

void RoverWheelDrive::setRefSpeed(double speed)
{
    qDebug()<<"RoverWheelDrive::setRefSpeed "<<speed;
    uint16_t zeroNativeSpeed = params[(int)RefSpeed]->defaultValue;
    if(speed > 0.05)//forward
        params[(int)RefSpeed]->setValue(zeroNativeSpeed+_fwdNativeRefSpeed);
    else
        if(speed < -0.05)//backward
            params[(int)RefSpeed]->setValue(zeroNativeSpeed-_bkwNativeRefSpeed);
    else
            params[(int)RefSpeed]->setDefaultValue();
}
