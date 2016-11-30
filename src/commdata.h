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
    uint16_t changedValue;
    uint16_t updatedValue;
    bool changed;
    bool updated;
    bool isChanged();
    bool isUpdated();
    bool isValid();
    uint16_t value();
    void setValue(uint16_t val);
    void updateValue(uint16_t val);
    void revertChanged();
    void revertUpdated();
    void commitChanged();
    void commitUpdated();
    uint8_t readCode;
    uint8_t writeCode;
    QByteArray readReq();
    QByteArray writeReq();
    ErrCode fromReq(const QByteArray& data, uint8_t shift=0);
    bool autoWrite;
    uint16_t defaultValue;
    bool autoUpdate;
    bool checkAfterWrite;
    void setDefaultValue();
    uint16_t autoUpdatePeriod;
    uint64_t autoUpdateLastTime;
    uint16_t autoWritePeriod;
    uint64_t autoWriteLastTime;
};

#endif // COMMDATA_H
