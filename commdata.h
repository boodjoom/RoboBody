#ifndef COMMDATA_H
#define COMMDATA_H

#include "stdint.h"
#include"QByteArray"

#include "errcode.h"

class CommData
{
public:
    CommData();
    uint16_t newValue;
    uint16_t curValue;
    uint16_t oldValue;
    bool changed;
    bool updated;
    bool isChanged();
    bool isUpdated();
    bool isValid();
    uint16_t value();
    void setValue(uint16_t val);
    void updateValue(uint16_t val);
    void commit();
    void submit();
    uint8_t readCode;
    uint8_t writeCode;
    QByteArray readReq();
    QByteArray writeReq();
    ErrCode fromReq(const QByteArray& data, uint8_t shift=0);
    bool autoWrite;
    uint16_t defaultValue;
    bool autoUpdate;
    bool checkAfterWrite;
};

#endif // COMMDATA_H
