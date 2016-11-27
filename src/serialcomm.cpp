#include "serialcomm.h"
#include "rovermodel.h"
#include "QThread"
extern "C"{
#include "crc16.h"
}
#include <QMessageLogger>
#include <QMutex>
#include <QCoreApplication>
#include <QElapsedTimer>

//#define SERIAL_DEBUG

QString toString(const QByteArray& data)
{
    QString str;
    for(int i=0;i<data.size();++i)
        str+=QString("%1 ").arg((uint8_t)data[i],2,16);
    return str;
}

SerialComm::SerialComm(QObject *parent)
    : QObject(parent)
    , _opened(false)
{
#ifndef USE_EXT_SERIALPORT
    _port = new QSerialPort(this);
#else
    _port = new QextSerialPort(QextSerialPort::Polling,this);
#endif
}

SerialComm::~SerialComm()
{

}

void SerialComm::setPortName(const QString &name){
    portName=name;
    _port->setPortName(portName);

}

void SerialComm::setBoudRate(int rate)
{
    boudRate=rate;
}

bool SerialComm::isOpened()
{
    return _opened;
}

void SerialComm::open()
{
    qDebug()<<"serialcomm open TH "<<QThread::currentThreadId();
    _opened=true;
}

void SerialComm::close()
{
    if(true && _port->isOpen())
    {
        _port->close();
        emit closed();
    }
    else
        emit warning(WarAllreadyClosed);
}

void SerialComm::stop()
{
    stopFlag=true;
}

void SerialComm::start()
{
    qDebug()<<"start serialcomm TH "<<QThread::currentThreadId();
//    _port = new QSerialPort(portName);
//    _port->setBaudRate(boudRate);
    stopFlag=false;
    QTimer::singleShot(10,this,SLOT(run()));
}

void SerialComm::run()
{
    //qDebug()<<"serialcomm run TH "<<QThread::currentThreadId();
//    while(!stopFlag)
//    {

        if(!_opened)
        {
            //qDebug()<<"serialcomm run port not ready";
            //QThread::msleep(100);
            QTimer::singleShot(100,this,SLOT(run()));
            return;
        }
        else if(!_port->isOpen())
        {
            if(!openImpl())
            {
                _opened=false;
                return;
            }
        }
        if(!_port->isOpen())
            return;
        //qDebug()<<"serialcomm run start sync total devices "<<model->devices.size();
        model->toFront();
#if 0
        //if(!model->hasNext())qDebug()<<"model is empty";
        while(model->hasNext())//для всех устройств
        {
            QPair<int, AbstractDevice*> devItem = model->next();
            //qDebug()<<"dev "<<devItem.first;
            AbstractDevice* dev = devItem.second;
            dev->toFront();
            while(dev->hasNext())//для всех параметров
            {
                QPair<int, CommData*> paramItem = dev->next();
                //qDebug()<<"param "<<paramItem.first;
                CommData* param = paramItem.second;
                if(param->isChanged())
                {
                    qDebug()<<"dev "<<devItem.first<<"param "<<paramItem.first<<" write changed";
                    ErrCode err = write(dev->prefix()+param->writeReq());
                    if(err==ErrOk)
                    {
                        if(param->checkAfterWrite)
                        {
                            qDebug()<<"Test read after write";
                            param->fromReq(dev->stripPrefix(read(dev->prefix()+param->readReq(),dev->dataLen)));
                            if(param->isUpdated())
                            {
                                qDebug()<<"Test read failed, actual value="<<param->value();
                                emit error(ErrSetParamFail);
                                param->revertUpdated();
                            }
                            else
                            {
                                qDebug()<<"Test read OK, actual value="<<param->value();
                                param->commitChanged();
                            }
                        }
                        else
                            param->commitChanged();
                    }
                    else
                    {
                        qDebug()<<"write failed";
                        emit error(ErrWriteFail);
                    }
                }
                else if(param->value()!=param->defaultValue && param->autoWrite)
                {
                    qDebug()<<"dev "<<devItem.first<<"param "<<paramItem.first<<" auto write";
                    write(dev->prefix()+param->writeReq());
                }
                else if(param->autoUpdate)
                {
                    //qDebug()<<"dev "<<devItem.first<<"param "<<paramItem.first<<" auto update "<<toString(param->readReq());
                    QByteArray answer = read(dev->prefix()+param->readReq(),dev->dataLen);
                    param->fromReq(dev->stripPrefix(answer));
                    qDebug()<<"dev "<<devItem.first<<"param "<<paramItem.first<<" value="<<param->value();
                }
            }
        }
#endif
        //QThread::msleep(50);
    if(stopFlag)
        emit finished();
    else
        QTimer::singleShot(50,this,SLOT(nextDev()));
//    }
//    emit finished();
}

ErrCode SerialComm::write(QByteArray data)
{
#ifdef SERIAL_DEBUG
    QElapsedTimer t;
    qDebug()<<"start write";
#endif
    if(false || !_port->isOpen())
        return ErrWrongState;

    addCrc(data);
#ifdef SERIAL_DEBUG
    t.start();
    qDebug()<<"write to port "<<toString(data);
#endif
    /*int writen = */_port->write(data);
    _port->flush();
    _port->waitForBytesWritten(-1);
    if(!_port->lastError())
    {
#ifdef SERIAL_DEBUG
        qDebug()<<"SerialComm: write ok, elapsed: "<<t.elapsed();
#endif
        return ErrOk;
    }
    else
    {
        qCritical()<<"SerialComm: write error: "<<_port->errorString();
        return ErrFail;
    }
}

void SerialComm::addCrc(QByteArray &data)
{
    //qDebug()<<"addCrc to data";
    uint8_t dataLen = data.length();
    uint16_t crc1 = crcCompute(reinterpret_cast<uint8_t*>(data.data()),dataLen);
    data.append((char)(crc1>>8));
    data.append((char)(crc1));
}

QByteArray SerialComm::read(QByteArray req, uint8_t dataBytesToRead, ErrCode *err)
{
    //qDebug()<<"start read";
    ErrCode errCode = ErrOk;
    if(false || !_port->isOpen())
    {
        errCode = ErrWrongState;
    }
    if(errCode == ErrOk)
    {
        _port->readAll();//flush read buffer
        errCode = write(req);
        if(errCode != ErrOk)
        {
             qCritical()<<"write req to port fail";
        }
    }
    QByteArray answer;
    uint8_t totalBytesToRead = dataBytesToRead+2;
    int retry;
    if(errCode == ErrOk)
    {
        retry=0;
#ifdef SERIAL_DEBUG
        QElapsedTimer t;
        t.start();
#endif
        while(retry<10 && answer.size()<totalBytesToRead)
        {
            int r = 20;
            while(r && !_port->bytesAvailable())
            {
                QThread::msleep(5);
                --r;
            }
            if(r)
            {
                answer += _port->read(totalBytesToRead-answer.size());
                removeLeadingZeros(answer);
            }
            ++retry;
        }
#ifdef SERIAL_DEBUG
        qDebug()<<"read from port "<<toString(answer)<<"retry="<<retry<<" elapsed="<<t.elapsed();
#endif
        if(!_port->lastError() && answer.size()==totalBytesToRead)
        {
#ifdef SERIAL_DEBUG
            qDebug()<<"read OK";
#endif
            errCode=ErrOk;
        }
        else
        {
            errCode=ErrFail;
            qCritical()<<"read from port fail, error: "<<_port->errorString();
            if(!_port->lastError())
            {
                qInfo()<<"Check device params";
            }
        }
    }
    if(errCode == ErrOk)
    {
        if(answer.length()>totalBytesToRead)
            answer = answer.left(totalBytesToRead);
        if(!checkCrc(answer))
        {
            qCritical()<<"SerialComm: wrong crc";
            errCode = ErrWrongCrc;
            answer.clear();
        }
        else
        {
            answer = answer.left(dataBytesToRead);
        }
    }
    if(err)
        (*err) = errCode;
    return answer;
}

bool SerialComm::checkCrc(QByteArray &data)
{
    //qDebug()<<"check crc data:"<<toString(data);
    uint8_t dataLen = data.length()-2;
    uint16_t crc1 = crcCompute(reinterpret_cast<uint8_t*>(data.data()),dataLen);
    uint16_t crc2 = (static_cast<uint8_t>(data[dataLen])<<8) | (static_cast<uint8_t>(data[dataLen+1]));
    //qDebug()<<"crc1="<<crc1<<" crc2="<<crc2;
    return (crc1 == crc2);
}

void SerialComm::removeLeadingZeros(QByteArray &data)
{
    uint16_t i;
    for(i=0;i<data.length();i++)
        if(data[i]!='\0')break;
    if(i>0)
        data=data.right(data.length()-i);
}

bool SerialComm::openImpl()
{
        if(_port->isOpen())
        {
            emit warning(WarAllreadyOpened);
        } else
        if(!_port->open(QIODevice::ReadWrite))
        {
            emit error(ErrOpenFail);
            qFatal("port %s open error",portName.toLatin1().data());
            std::abort();
        }
        else
        {

#ifndef USE_EXT_SERIALPORT
            _port->setBaudRate(boudRate);
            _port->setParity(QSerialPort::NoParity);
            _port->setStopBits(QSerialPort::OneStop);
            _port->setFlowControl(QSerialPort::NoFlowControl);
            _port->setDataBits(QSerialPort::Data8);
#else
            _port->setBaudRate(static_cast<BaudRateType>(boudRate));
            _port->setParity(PAR_NONE);
            _port->setDataBits(DATA_8);
            _port->setStopBits(STOP_1);
            //port.setBreakEnabled(false);
            _port->setFlowControl(FLOW_OFF);
            _port->setTextModeEnabled(false);
#endif
            emit opened();
            qInfo()<<"port "<<portName<<" opened";
            return true;
        }
        return false;
}

void SerialComm::nextDev()
{
    if(model->hasNext())
    {
        QPair<int, AbstractDevice*> devItem = model->next();
        devItem.second->toFront();
        if(devItem.second->isValid())
            QTimer::singleShot(10,this,SLOT(nextParam()));
        else
        {
            qWarning()<<"device "<<devItem.first<<" is not valid";
            QTimer::singleShot(1,this,SLOT(nextDev()));
        }
    }
    else
    {
        QTimer::singleShot(10,this,SLOT(run()));
    }
}

void SerialComm::nextParam()
{
    QPair<int, AbstractDevice*> devItem = model->current();
    AbstractDevice* dev = devItem.second;
    if(dev->hasNext())
    {
        int nextParamTimeout=10;
        QPair<int, CommData*> paramItem = dev->next();
//        qDebug()<<"param "<<paramItem.first;
        CommData* param = paramItem.second;
        if(param->isChanged())
        {
            qDebug()<<"dev "<<devItem.first<<"param "<<paramItem.first<<" write changed";
            ErrCode err = write(dev->prefix()+param->writeReq());
            if(err==ErrOk)
            {
                if(param->checkAfterWrite)
                {
#ifdef SERIAL_DEBUG
                    qDebug()<<"Test read after write";
#endif
                    param->fromReq(dev->stripPrefix(read(dev->prefix()+param->readReq(),dev->dataLen)));
                    if(param->isUpdated())
                    {
                        qCritical()<<"Test read failed, actual value="<<param->value();
                        emit error(ErrSetParamFail);
                        param->revertUpdated();
                    }
                    else
                    {
#ifdef SERIAL_DEBUG
                        qDebug()<<"Test read OK, actual value="<<param->value();
#endif
                        param->commitChanged();
                    }
                }
                else
                    param->commitChanged();
            }
            else
            {
                qCritical()<<"SerialComm: write request failed";
                emit error(ErrWriteFail);
            }
        }
        else if(param->value()!=param->defaultValue && param->autoWrite)
        {
            qDebug()<<"dev "<<devItem.first<<"param "<<paramItem.first<<" auto write";
            write(dev->prefix()+param->writeReq());
        }
        else if(param->autoUpdate)
        {
            //qDebug()<<"dev "<<devItem.first<<"param "<<paramItem.first<<" auto update "<<toString(param->readReq());
            QByteArray answer = read(dev->prefix()+param->readReq(),dev->dataLen);
            param->fromReq(dev->stripPrefix(answer));
            qDebug()<<"dev "<<devItem.first<<"param "<<paramItem.first<<" value="<<param->value();
        } else //nothing to be done
            nextParamTimeout=1;
        QTimer::singleShot(nextParamTimeout,this,SLOT(nextParam()));

    }
    else
    {
        QTimer::singleShot(10,this,SLOT(nextDev()));
    }
}
