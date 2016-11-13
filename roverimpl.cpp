#include "roverimpl.h"
#include <QDateTime>
#include <QSettings>
#include <QVariant>
#include "serialcomm.h"
#include "rovermodel.h"
#include <QThread>
#include <QDebug>

RoverImpl::RoverImpl()
{

}

ErrCode RoverImpl::init(QSettings &settings)
{
    qDebug()<<"start rover init TH "<<QThread::currentThreadId();
    settings.beginGroup("Rover");
    _roverModel = new RoverModel();
    _serialComm = new SerialComm();
    _serialComm->setModel(_roverModel);
    const QString portName = settings.value("PortName","\\\\.\\COM4").toString();
    _serialComm->setPortName(portName);
    const int boudRate = settings.value("PortRate",9600).toInt();
    _serialComm->setBoudRate(boudRate);
    _commThread = new QThread();
    _serialComm->moveToThread(_commThread);
    connect(this,SIGNAL(open()),_serialComm, SLOT(open()),Qt::QueuedConnection);
    connect(this,SIGNAL(close()),_serialComm,SLOT(close()),Qt::QueuedConnection);
    connect(this,SIGNAL(stop()),_serialComm,SLOT(stop()),Qt::QueuedConnection);
    connect(_serialComm, SIGNAL(opened()),this,SIGNAL(opened()),Qt::QueuedConnection);
    connect(_serialComm, SIGNAL(closed()),this,SIGNAL(closed()),Qt::QueuedConnection);
    connect(_commThread, SIGNAL(started()),_serialComm, SLOT(start()),Qt::QueuedConnection);
    connect(_serialComm, SIGNAL(finished()), _serialComm, SLOT(deleteLater()),Qt::QueuedConnection);
    connect(_commThread, SIGNAL(finished()), _commThread, SLOT(deleteLater()),Qt::QueuedConnection);

    settings.endGroup();
    qDebug()<<"start thread TH "<<QThread::currentThreadId();
    //_serialComm->start();
    _commThread->start();
    return ErrOk;
}

void RoverImpl::setSpeed(double speed, QDateTime timeout)
{
    if(!qFuzzyCompare(speed+1.0,_speed+1.0))
    {
        _speed=speed;
        //set model values
    }
}

void RoverImpl::deInit()
{

}

void RoverImpl::openPort()
{
    qDebug()<<"send openport TH "<<QThread::currentThreadId();
    //_serialComm->open();
    emit open();
}

void RoverImpl::closePort()
{
    emit close();
}
