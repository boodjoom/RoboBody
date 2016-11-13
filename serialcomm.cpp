#include "serialcomm.h"
#include "rovermodel.h"
#include "QThread"
extern "C"{
#include "crc16.h"
}

SerialComm::SerialComm(QObject *parent) : QObject(parent)
{

}

void SerialComm::open()
{
    if(_port != nullptr && _port->isOpen())
    {
        emit warning(WarAllreadyOpened);
        return;
    }
    _port = new QSerialPort(portName,this);
    _port->setBaudRate(boudRate);
    if(!_port->open(QIODevice::ReadWrite))
    {
        delete _port;
        _port = nullptr;
        emit error(ErrOpenFail);
    }
    else
        emit opened();
}

void SerialComm::close()
{
    if(_port != nullptr && _port->isOpen())
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
    stopFlag=false;
    run();
}

void SerialComm::run()
{
    while(!stopFlag)
    {
        if(_port == nullptr || !_port->isOpen())
        {
            QThread::msleep(100);
            continue;
        }
        model->toFront();
        while(model->hasNext())//для всех устройств
        {
            QPair<int, AbstractDevice*> devItem = model->next();
            AbstractDevice* dev = devItem.second;
            dev->toFront();
            while(dev->hasNext())//для всех параметров
            {
                QPair<int, CommData*> paramItem = dev->next();
                CommData* param = paramItem.second;
                if(param->isChanged())
                {
                    write(dev->prefix()+param->writeReq());
                    param->commit();
                }
                else if(param->value()!=param->defaultValue && param->autoWrite)
                {
                    write(dev->prefix()+param->writeReq());
                }
                else if(param->autoUpdate)
                {
                    QByteArray answer = read(dev->prefix()+param->writeReq(),dev->dataLen);
                    param->fromReq(dev->stripPrefix(answer));
                }
            }
        }
        QThread::msleep(50);
    }
    emit finished();
}

ErrCode SerialComm::write(QByteArray data)
{
    if(_port == nullptr || !_port->isOpen())
        return ErrWrongState;
    addCrc(data);
    _port->write(data);
    int retry=0;
    while(retry<10)
    {
        if(_port->waitForBytesWritten(10))
            break;
        else
            ++retry;
    }
    if(retry<10)
        return ErrOk;
    else
        return ErrFail;
}

void SerialComm::addCrc(QByteArray &data)
{
    uint8_t dataLen = data.length();
    uint16_t crc1 = crcCompute(reinterpret_cast<uint8_t*>(data.data()),dataLen);
    data.append((char)(crc1>>8));
    data.append((char)(crc1));
}

QByteArray SerialComm::read(QByteArray req, uint8_t dataBytesToRead, ErrCode *err)
{
    ErrCode errCode = ErrOk;
    if(_port == nullptr || !_port->isOpen())
    {
        errCode = ErrWrongState;
    }
    if(errCode == ErrOk)
    {
        _port->readAll();
        errCode = write(req);
    }
    QByteArray answer;
    uint8_t totalBytesToRead = dataBytesToRead+2;
    int retry;
    if(errCode == ErrOk)
    {
        retry=0;
        while(retry<10)
        {
            _port->waitForReadyRead(10);
            answer += _port->readAll();
            removeLeadingZeros(answer);
            if(answer.length()>=totalBytesToRead)
                break;
            else
                ++retry;
        }
        if(retry<10)
            errCode=ErrOk;
        else
            errCode=ErrFail;
    }
    if(errCode == ErrOk)
    {
        if(answer.length()>totalBytesToRead)
            answer = answer.left(totalBytesToRead);
        if(!checkCrc(answer))
        {
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
    uint8_t dataLen = data.length()-2;
    uint16_t crc1 = crcCompute(reinterpret_cast<uint8_t*>(data.data()),dataLen);
    uint16_t crc2 = (static_cast<uint16_t>(data[dataLen])<<8) | (static_cast<uint16_t>(data[dataLen+1]));
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
