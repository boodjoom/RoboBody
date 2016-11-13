#ifndef COMMDATA_H
#define COMMDATA_H

#include "stdint.h"
#include"QByteArray"

#include "errcode.h"

class CommData
{
public:
    CommData();
    uint16_t curValue;
    uint16_t oldValue;
    bool changed;
    bool updated;
    bool isChanged()
    {return changed;}
    bool isUpdated()
    {return updated;}
    bool isValid();
    uint16_t value()
    {return curValue;}
    void setValue(uint16_t val)
    {if(val != curValue){oldValue=curValue;curValue=val;changed=true;}}
    void updateValue(uint16_t val)
    {if(val != curValue){oldValue=curValue;curValue=val;updated=true;}}
    void commit()
    {if(changed){changed=false;}}
    void submit()
    {if(updated){updated=false;}}
    uint8_t readCode;
    uint8_t writeCode;
    QByteArray readReq(){char buf[] = {(char)readCode,'\0','\0'}; return QByteArray::fromRawData(buf,3);}
    QByteArray writeReq(){char buf[] = {(char)writeCode,(char)(value()>>8),(char)value()};return QByteArray::fromRawData(buf,3);}
    ErrCode fromReq(const QByteArray& data, uint8_t shift=0)
    {
        if(static_cast<uint8_t>(data[shift])!=readCode && static_cast<uint8_t>(data[shift])!=writeCode)
            return ErrWrongCode;
        uint16_t val = (static_cast<uint16_t>(data[shift+1])<<8) | (static_cast<uint16_t>(data[shift+2]));
        updateValue(val);
        return ErrOk;
    }
    bool autoWrite;
    uint16_t defaultValue;
    bool autoUpdate;
};

#endif // COMMDATA_H
