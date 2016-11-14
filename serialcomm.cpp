#include "serialcomm.h"
#include "rovermodel.h"
#include "QThread"
extern "C"{
#include "crc16.h"
}
#include <QMessageLogger>
#include <QMutex>
#include <QCoreApplication>

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
    _port = new QSerialPort(this);
}

SerialComm::~SerialComm()
{

}

void SerialComm::setPortName(const QString &name){
    portName=name;
    _port->setPortName(portName);

}

void SerialComm::setBoudRate(int rate){boudRate=rate;_port->setBaudRate(boudRate);}

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
//    qDebug()<<"start write";
    if(false || !_port->isOpen())
        return ErrWrongState;
//    portMutex.lock();
    addCrc(data);
//    qDebug()<<"write to port "<<toString(data);
    /*int writen = */_port->write(data);
//    for(int i = 0;i<data.length();++i)
//    {
//        char buf = data[i];
//        _port->write(&buf,1);
//        QThread::msleep(1);
//    }
    //qDebug()<<"Writen="<<writen;
    int retry=0;
//    _port->flush();
//    while (!_port->waitForBytesWritten(10));
    while(retry<10)
    {
        if(_port->waitForBytesWritten(10))
            break;
        else
            ++retry;
    }
//    qDebug()<<"Writing dobne retry="<<retry;
//    portMutex.unlock();
//    if(retry == 0)//скорее всего ничего не успели сделать
//    {
//        qDebug()<<"additional timeout";
        //QThread::msleep(30);
//    }
    //QCoreApplication::processEvents();
    if(retry<10)
        return ErrOk;
    else
    {
        _port->flush();
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
        //_port->readAll();
        errCode = write(req);
        if(errCode != ErrOk)
        {
             qDebug()<<"write req to port fail";
        }
    }
    QByteArray answer;
    uint8_t totalBytesToRead = dataBytesToRead+2;
    int retry;
    if(errCode == ErrOk)
    {
        retry=0;
        while(retry<10)
        {
            if(_port->waitForReadyRead(30))
            {
            answer += _port->readAll();
            removeLeadingZeros(answer);
            if(answer.length()>=totalBytesToRead)
                break;
            else
                ++retry;
            }
        }
        //qDebug()<<"read from port "<<toString(answer)<<"retry="<<retry;
        if(retry<10)
            errCode=ErrOk;
        else
        {
            errCode=ErrFail;
            qDebug()<<"read from port fail";
        }
    }
    if(errCode == ErrOk)
    {
        if(answer.length()>totalBytesToRead)
            answer = answer.left(totalBytesToRead);
        if(!checkCrc(answer))
        {
            //qDebug()<<"crc error";
            errCode = ErrWrongCrc;
            answer.clear();
        }
        else
        {
            //qDebug()<<"strip crc data:"<<toString(answer);
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
            _port->setBaudRate(boudRate);
            _port->setParity(QSerialPort::NoParity);
            _port->setStopBits(QSerialPort::OneStop);
            _port->setFlowControl(QSerialPort::NoFlowControl);
            _port->setDataBits(QSerialPort::Data8);
            emit opened();
            qInfo()<<"port "<<portName<<" opened";
            return true;
        }
        return false;
        //    portMutex.unlock();
}

void SerialComm::nextDev()
{
    if(model->hasNext())
    {
        model->next().second->toFront();
        QTimer::singleShot(30,this,SLOT(nextParam()));
    }
    else
    {
        QTimer::singleShot(30,this,SLOT(run()));
    }
}

void SerialComm::nextParam()
{
    QPair<int, AbstractDevice*> devItem = model->current();
    AbstractDevice* dev = devItem.second;
    if(dev->hasNext())
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
        QTimer::singleShot(30,this,SLOT(nextParam()));
    }
    else
    {
        QTimer::singleShot(30,this,SLOT(nextDev()));
    }
}
