#include "roverimpl.h"
#include <QDateTime>
#include <QSettings>
#include <QVariant>
#include "serialcomm.h"
#include "rovermodel.h"
#include <QThread>

RoverImpl::RoverImpl()
{

}

void RoverImpl::init(QSettings &settings)
{
    settings.beginGroup("Rover");

    _roverModel = new RoverModel();
    _serialComm = new SerialComm();
    _serialComm->setModel(_roverModel);
    const QString portName = settings.value("PortName","\\.\COM4").toString();
    _serialComm->setPortName(portName);
    const int boudRate = settings.value("PortRate",9600).toInt();
    _serialComm->setBoudRate(boudRate);
    _commThread = new QThread();
    _serialComm->moveToThread(_commThread);
    connect(this,SIGNAL(open()),_serialComm, SLOT(open()),Qt::QueuedConnection);
    connect(this,SIGNAL(close()),_serialComm,SLOT(close()),Qt::QueuedConnection);
    connect(this,SIGNAL(stop()),_serialComm,SLOT(stop()),Qt::QueuedConnection);
    connect(_commThread, SIGNAL(started()),_serialComm, SLOT(start()));
    connect(_serialComm, SIGNAL(finished()), _serialComm, SLOT(deleteLater()));
    connect(_commThread, SIGNAL(finished()), _commThread, SLOT(deleteLater()));

    settings.endGroup();
}

void RoverImpl::setSpeed(double speed, QDateTime timeout)
{
    if(!qFuzzyCompare(speed+1.0,_speed+1.0))
    {
        _speed=speed;
        //set model values
    }
}
