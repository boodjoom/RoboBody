#ifndef ABSTRACTDEVICE_H
#define ABSTRACTDEVICE_H

#include "commdata.h"
#include <QHash>

class AbstractDevice
{
public:
    enum DeviceParams
    {
        RefValue =1,
        CurValue =2
    };
    AbstractDevice();
    QHash<int, CommData*> params;
    bool hasNext();
    QPair<int, CommData*> next();
    void toFront();
    QHashIterator<int, CommData*> paramIter;
    virtual QByteArray prefix()=0;
    virtual QByteArray stripPrefix(const QByteArray& data)=0;
    uint8_t dataLen;
};

#endif // ABSTRACTDEVICE_H
