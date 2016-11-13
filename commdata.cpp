#include "commdata.h"
#include <QDebug>

CommData::CommData()
{

}

bool CommData::isChanged()
{return changed;}

bool CommData::isUpdated()
{return updated;}

uint16_t CommData::value()
{return curValue;}

void CommData::setValue(uint16_t val)
{if(val != curValue)    {changedValue=curValue;curValue=val;changed=true;}}

void CommData::updateValue(uint16_t val)
{if(val != curValue){updatedValue=curValue;curValue=val;updated=true;}}

void CommData::revertChanged()
{
    if(changed)
    {
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
{if(changed){changed=false;}}

void CommData::commitUpdated()
{if(updated){updated=false;}}

QByteArray CommData::readReq()
{
    char buf[] = {(char)readCode,'\0','\0'};
    return QByteArray(buf,3);
}

QByteArray CommData::writeReq()
{
    //qDebug()<<"writeReq code="<<writeCode<<" val="<<value();
    char buf[] = {(char)writeCode,(char)(value()>>8),(char)value()};
    return QByteArray(buf,3);
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
