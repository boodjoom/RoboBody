#include "commdata.h"
#include <QDebug>

CommData::CommData():
    curValue(55555)
  , changedValue(55555)
  , updatedValue(55555)
  , autoUpdatePeriod(0)
  , autoUpdateLastTime(0)
{

}

bool CommData::isChanged()
{return changed;}

bool CommData::isUpdated()
{return updated;}

uint16_t CommData::value()
{return curValue;}

void CommData::setValue(uint16_t val)
{
    if(val != curValue)
    {
        qDebug()<<"CommData::setValue "<<val;
        changedValue=curValue;
        curValue=val;
        changed=true;
    }
}

void CommData::updateValue(uint16_t val)
{if(val != curValue){updatedValue=curValue;curValue=val;updated=true;}}

void CommData::revertChanged()
{
    if(changed)
    {
        qDebug()<<"CommData::revertChanged";
        curValue = changedValue;
        changed = false;
    }
}

void CommData::revertUpdated()
{
    if(updated)
    {
        curValue = updatedValue;
        updated = false;
    }
}

void CommData::commitChanged()
{if(changed){qDebug()<<"CommData::commitChanged";changed=false;}}

void CommData::commitUpdated()
{if(updated){updated=false;}}

QByteArray CommData::readReq()
{
    Q_ASSERT(readCode);
    QByteArray req;
    if(readCode)
    {
        req.append((char)readCode);
    }
    req.append('\0');
    req.append('\0');

    return req;
}

QByteArray CommData::writeReq()
{
    QByteArray req;
    if(writeCode)
    {
        req.append((char)writeCode);
    }
    req.append((char)(value()>>8));
    req.append((char)value());
    return req;
}

QString toString1(const QByteArray& data)
{
    QString str;
    for(int i=0;i<data.size();++i)
        str+=QString("%1 ").arg((uint8_t)data[i],2,16);
    return str;
}

ErrCode CommData::fromReq(const QByteArray &data, uint8_t shift)
{
    Q_ASSERT(readCode);
    //qDebug()<<"fromReq data:"<<toString1(data);
    if(data.isEmpty() || data.size()<(shift+3))
    {
        //qDebug()<<"fromReq wrong data len="<<data.size();
        return ErrWrongData;
    }
    if(static_cast<uint8_t>(data[shift])!=readCode && static_cast<uint8_t>(data[shift])!=writeCode)
    {
        //qDebug()<<"fromReq wrong code";
        return ErrWrongCode;
    }
    //qDebug()<<"hi="<<QString::number((uint8_t)data[shift+1],16)<<" lo="<<QString::number((uint8_t)data[shift+2],16);
    uint16_t val = (static_cast<uint8_t>(data[shift+1])<<8) | (static_cast<uint8_t>(data[shift+2]));
    //qDebug()<<"val="<<QString::number(val,16);
    updateValue(val);
    return ErrOk;
}

void CommData::setDefaultValue()
{
    setValue(defaultValue);
}
