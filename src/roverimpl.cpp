#include "roverimpl.h"
#include <QDateTime>
#include <QSettings>
#include <QVariant>
#include "serialcomm.h"
#include "rovermodel.h"
#include <QThread>
#include <QDebug>
#include <QTimer>
#include <QMetaEnum>

RoverImpl::RoverImpl()
    : QObject()
    , _speed(0.0)
    , _yaw(0.0)
    , _distanceApproximation(0.0)
    , _intervalCorrection(0.0)
    , _speedApproximation(0.0)
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
    bool ok;
    const QString portName = settings.value("PortName","\\\\.\\COM4").toString();
    const int boudRate = settings.value("PortRate",9600).toInt();
    _intervalCorrection = settings.value("VehicleIntervalCorrection",0.4).toDouble(&ok);
    if(!ok)
        _intervalCorrection=0.4;
    _speedApproximation = settings.value("VehicleSpeedAproximation",1.0).toDouble(&ok);
    if(!ok)
        _speedApproximation=1.0;
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


    connect(&_manipStageTimer,SIGNAL(timeout()),this,SLOT(onManipStageTimeout()));
    //qDebug()<<"start thread TH "<<QThread::currentThreadId();
    //_serialComm->start();
    _commThread->start();
    return ErrOk;
}

bool isZero(double value)
{
    return value > -0.05 && value <0.05;
}

bool isPositive(double value)
{
    return value > 0.05;
}

bool isNegative(double value)
{
    return value < 0.05;
}

void RoverImpl::setRefSpeed(double speed, quint64 timeout)
{
//    qDebug()<<"setSpeed "<<speed;
    if(!qFuzzyCompare(speed+1.0,_speed+1.0))
    {
        if(!isZero(speed) && isZero(_speed))
        {
            _speedOnTimer.restart();
        } else
        if(isZero(speed) && isPositive(_speed))
        {
            _distanceApproximation += approximateDistance((0.001)*(double)_speedOnTimer.elapsed());
        } else
        if(isZero(speed) && isNegative(_speed))
        {
            _distanceApproximation += approximateDistance((0.001)*(double)_speedOnTimer.elapsed());
        }
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
    Q_UNUSED(enabled)
    qDebug()<<"Not i;mplemented";
    return ErrNotImplemented;
}

double RoverImpl::getRefSpeed(ErrCode *err)
{
    if(err)
        *err = ErrOk;
    return _speed;
}

double RoverImpl::getRefYaw(ErrCode *err)
{
    if(err)
        *err = ErrOk;
    return _yaw;
}

double RoverImpl::getTravel(ErrCode *err)
{
    if(err)
        *err = ErrOk;
    return _distanceApproximation;
}

void RoverImpl::startManip()
{
  qDebug()<<"RoverImpl::startManip";
  setCurManipStage(ManipStage::StartManip);
  QMetaObject::invokeMethod(this,"onManipStageTimeout");
}

ManipState RoverImpl::getManipState()
{
  qDebug()<<"RoverImpl::getManipState";
  ManipState result = ManipState::Unknown;
  if(_roverModel->getManipGripperState() == GripperState::Moving)
      result = ManipState::Moving;
  else
      result = _roverModel->getManipState();
  return result;
}

double RoverImpl::getBattery(Battery battery, ErrCode *err)
{
    ErrCode e = ErrOk;
    double result = 0.0;
    switch(battery)
    {
    case Battery::Body:
        result = _roverModel->getSensor(RoverModel::BodyBatterySensor);
        break;
    case Battery::Brain:
        result = _roverModel->getSensor(RoverModel::BrainBatterySensor);
        break;
    default:
        e = ErrWrongArg;
        break;
    };
    if(err)
    {
        *err = e;
    }
    return result;
}

void RoverImpl::onRefSpeedTimeout()
{
    setRefSpeed(0,0);
}

void RoverImpl::onManipStageTimeout()
{
    switch(_curManipStage)
    {
    case ManipStage::StartManip:
        _roverModel->setManipPose(ManipPose::Target);
        setCurManipStage(ManipStage::MoveToTarget);
        _manipStageTimer.start(250);
        break;
    case ManipStage::MoveToTarget:
        if(_roverModel->getManipState() == ManipState::AtTarget)
        {
             setCurManipStage(ManipStage::CloseGripper);
             _roverModel->setManipGripperPose(GripperPose::Closed);
        }
        break;
    case ManipStage::CloseGripper:
        if(_roverModel->getManipGripperState() == GripperState::Closed)
        {
            setCurManipStage(ManipStage::MoveToHome);
            _roverModel->setManipPose(ManipPose::Home);
        }
        break;
    case ManipStage::MoveToHome:
        if(_roverModel->getManipState() == ManipState::AtHome)
        {
            setCurManipStage(ManipStage::OpenGripper);
            _roverModel->setManipGripperPose(GripperPose::Opened);
        }
        break;
    case ManipStage::OpenGripper:
        if(_roverModel->getManipGripperState() == GripperState::Opened)
        {
            _manipStageTimer.stop();
            setCurManipStage(ManipStage::StopManip);
        }
        break;
     default:break;
    };
}

double RoverImpl::approximateDistance(double newSpeedOnInterval)
{
    return (newSpeedOnInterval - _intervalCorrection)*_speedApproximation;
}

QString RoverImpl::toString(RoverImpl::ManipStage stage)
{
    static int enumIdx = RoverImpl::staticMetaObject.indexOfEnumerator("ManipStage");
    return RoverImpl::staticMetaObject.enumerator(enumIdx).valueToKey((int)stage);
}

void RoverImpl::setCurManipStage(RoverImpl::ManipStage stage)
{
    if(stage != _curManipStage)
    {
        qDebug()<<"RoverImpl: ManipStage changed old:"<<toString(_curManipStage)<<" new:"<<toString(stage);
        _curManipStage=stage;
    }
}

void RoverImpl::execManip(ManipAction action)
{
  switch(action)
  {
    case ManipAction::Start: startManip();break;
    case ManipAction::Stop: _manipStageTimer.stop();
            setCurManipStage(ManipStage::StopManip);
            break;
    case ManipAction::Home: _roverModel->setManipPose(ManipPose::Home); break;
    case ManipAction::Target: _roverModel->setManipPose(ManipPose::Target); break;
    case ManipAction::Base: _roverModel->setManipPose(ManipPose::Base); break;
    case ManipAction::Open: _roverModel->setManipGripperPose(GripperPose::Opened); break;
    case ManipAction::Close: _roverModel->setManipGripperPose(GripperPose::Closed); break;
    default: break;
  };
}
