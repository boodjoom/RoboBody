#include "roverimpl.h"
#include <QDateTime>
#include <QSettings>
#include <QVariant>
#include "serialcomm.h"
#include "rovermodel.h"
#include <QThread>
#include <QDebug>
#include <QTimer>

RoverImpl::RoverImpl()
    : QObject()
    , _speed(0.0)
    , _yaw(0.0)
{
    _refSpeedTimer = new QTimer(this);
    connect(_refSpeedTimer,SIGNAL(timeout()), this, SLOT(onRefSpeedTimeout()));
}

ErrCode RoverImpl::init(QSettings &settings)
{
    qDebug()<<"start rover init TH "<<QThread::currentThreadId();

    _roverModel = new RoverModel(settings);
    _serialComm = new SerialComm();
    _serialComm->setModel(_roverModel);
    settings.beginGroup("Rover");
    const QString portName = settings.value("PortName","\\\\.\\COM4").toString();
    const int boudRate = settings.value("PortRate",9600).toInt();
    settings.endGroup();
    qInfo()<<"Rover PortName = "<<portName;
    _serialComm->setPortName(portName);
    qInfo()<<"Rover PortRate = "<<boudRate;
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
    connect(_serialComm, SIGNAL(error(ErrCode)),this,SIGNAL(error(ErrCode)));
    connect(_serialComm, SIGNAL(warning(WarCode)),this,SIGNAL(warning(WarCode)));

    //qDebug()<<"start thread TH "<<QThread::currentThreadId();
    //_serialComm->start();
    _commThread->start();
    return ErrOk;
}

void RoverImpl::setRefSpeed(double speed, quint64 timeout)
{
//    qDebug()<<"setSpeed "<<speed;
    if(!qFuzzyCompare(speed+1.0,_speed+1.0))
    {
        _speed=speed;
        _roverModel->setRefSpeed(_speed);
    }
    if(timeout>0)
    {
        _refSpeedTimer->setInterval(timeout);
        QMetaObject::invokeMethod(_refSpeedTimer,"start");
    }
    else
        QMetaObject::invokeMethod(_refSpeedTimer,"stop");
}

void RoverImpl::setRefYaw(double yaw, quint64 timeout)
{
//    qDebug()<<"setYaw "<<yaw;
    Q_UNUSED(timeout)
    if(!qFuzzyCompare(yaw+1.0,_yaw+1.0))
    {
        _yaw=yaw;
        _roverModel->setRefAngle(_yaw);
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

ErrCode RoverImpl::setBreaks(bool enabled)
{
    qDebug()<<"Not implemented";
    return ErrNotImplemented;
}

double RoverImpl::getRefSpeed(ErrCode *err)
{
    return _speed;
}

double RoverImpl::getRefYaw(ErrCode *err)
{
    return _yaw;
}

void RoverImpl::onRefSpeedTimeout()
{
    setRefSpeed(0,0);
}
