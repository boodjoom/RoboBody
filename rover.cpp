#include "rover.h"
#include "roverimpl.h"
#include <QDateTime>

Rover::Rover()
{
    pImpl = new RoverImpl();
    QObject::connect(pImpl,SIGNAL(opened()),this,SIGNAL(connected()));
    QObject::connect(pImpl,SIGNAL(closed()),this,SIGNAL(disconnected()));
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

ErrCode Rover::setSpeed(double speed)
{
    pImpl->setSpeed(speed,QDateTime::currentDateTime());
}

void Rover::connect()
{
    pImpl->openPort();
}

void Rover::disconnect()
{
    pImpl->closePort();
}
