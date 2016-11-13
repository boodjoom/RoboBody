#include "rover.h"
#include "roverimpl.h"
#include <QDateTime>

Rover::Rover()
{
    pImpl = new RoverImpl();
    QObject::connect(pImpl,SIGNAL(opened()),this,SIGNAL(connected()));
    QObject::connect(pImpl,SIGNAL(closed()),this,SIGNAL(disconnected()));
    QObject::connect(pImpl,SIGNAL(error(ErrCode)),this,SIGNAL(error(ErrCode)));
    QObject::connect(pImpl,SIGNAL(warning(ErrCode)),this,SIGNAL(warning(ErrCode)));
}

Rover::~Rover()
{
    delete pImpl;
}

ErrCode Rover::init(QSettings &settings)
{
    return pImpl->init(settings);
}

ErrCode Rover::deInit()
{
    pImpl->deInit();
    return ErrOk;
}

ErrCode Rover::setRefSpeed(double speed)
{
    pImpl->setRefSpeed(speed,QDateTime::currentDateTime());
    return ErrOk;
}

double Rover::getCurSpeed(ErrCode *err)
{
}

double Rover::getRefSpeed(ErrCode *err)
{
}

bool Rover::isBreaksEnabled(ErrCode *err)
{

}

ErrCode Rover::setBreaksEnabled(bool enabled)
{
    return pImpl->setBreaks(enabled);
}

double Rover::getRefYaw(ErrCode *err)
{

}

double Rover::getCurYaw(ErrCode *err)
{

}

ErrCode Rover::setRefYaw(double yaw)
{
    pImpl->setRefYaw(yaw,QDateTime::currentDateTime());
}

void Rover::connect()
{
    pImpl->openPort();
}

void Rover::disconnect()
{
    pImpl->closePort();
}
