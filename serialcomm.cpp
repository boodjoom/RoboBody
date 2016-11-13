#include "serialcomm.h"
#include "rovermodel.h"
#include "QThread"

SerialComm::SerialComm(QObject *parent) : QObject(parent)
{

}

void SerialComm::run()
{
    while(!stopFlag)
    {
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
}

ErrCode SerialComm::write(QByteArray data)
{
    addCrc(data);
    //port->write();
    return ErrOk;
}

void SerialComm::addCrc(QByteArray &data)
{
    data.append('\0');
    data.append('\0');
}

QByteArray SerialComm::read(QByteArray req, uint8_t dataBytesToRead, ErrCode *err)
{
    ErrCode errCode;
    errCode = write(req);
    QByteArray answer;
    if(err)
        (*err) = errCode;
    //check crc, strip crc
    return answer;
}
