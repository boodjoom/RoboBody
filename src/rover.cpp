#include "rover.h"
#include "roverimpl.h"
#include <QDateTime>

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
    return 0.0;
}

double Rover::getRefSpeed(ErrCode *err)
{
    return pImpl->getRefSpeed(err);
}

bool Rover::isBreaksEnabled(ErrCode *err)
{
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
