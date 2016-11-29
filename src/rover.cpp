#include "rover.h"
#include "roverimpl.h"
#include <QDateTime>
#include <QMetaEnum>

Rover::Rover()
{
    pImpl = new RoverImpl();
    QObject::connect(pImpl,SIGNAL(opened()),this,SIGNAL(connected()));
    QObject::connect(pImpl,SIGNAL(closed()),this,SIGNAL(disconnected()));
    QObject::connect(pImpl,SIGNAL(error(ErrCode)),this,SIGNAL(error(ErrCode)));
    QObject::connect(pImpl,SIGNAL(warning(WarCode)),this,SIGNAL(warning(WarCode)));
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

ErrCode Rover::setRefSpeed(double speed, unsigned long long timeout)
{
    pImpl->setRefSpeed(speed,timeout);
    return ErrOk;
}

double Rover::getCurSpeed(ErrCode *err)
{
    if(err)
        *err = ErrNotImplemented;
    return 0.0;
}

double Rover::getRefSpeed(ErrCode *err)
{
    return pImpl->getRefSpeed(err);
}

bool Rover::isBreaksEnabled(ErrCode *err)
{
    if(err)
        *err = ErrNotImplemented;
    return false;
}

ErrCode Rover::setBreaksEnabled(bool enabled)
{
    return pImpl->setBreaks(enabled);
}

double Rover::getRefYaw(ErrCode *err)
{
    return pImpl->getRefYaw(err);
}

double Rover::getCurYaw(ErrCode *err)
{
    if(err)
        *err = ErrNotImplemented;
    return 0.0;
}

double Rover::getTravel(ErrCode *err)
{
    return pImpl->getTravel(err);
}

void Rover::startManip()
{
    pImpl->startManip();
}

ManipState Rover::getManipState()
{
    return pImpl->getManipState();
}

QString Rover::toString(ManipState state)
{
    switch(state)
    {
    case ManipState::AtHome:return "home";
    case ManipState::AtTarget:return "target";
    case ManipState::AtBase:return "base";
    case ManipState::Moving:return "moving";
    case ManipState::Idle:return "idle";
    default: return "unknown";
    };
}

double Rover::getBattery(ErrCode *err)
{
    return pImpl->getBattery(err);
}

ErrCode Rover::setRefYaw(double yaw)
{
    pImpl->setRefYaw(yaw,0);
    return ErrOk;
}

void Rover::connect()
{
    pImpl->openPort();
}

void Rover::disconnect()
{
    pImpl->closePort();
}
