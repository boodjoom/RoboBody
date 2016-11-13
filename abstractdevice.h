#ifndef ABSTRACTDEVICE_H
#define ABSTRACTDEVICE_H

#include "commdata.h"
#include <QHash>

class AbstractDevice
{
public:
    AbstractDevice():paramIter(params){}
    QHash<int, CommData*> params;
    bool hasNext()
    {return paramIter.hasNext();}
    QPair<int, CommData*> next(){paramIter.next();return QPair<int,CommData*>(paramIter.key(),paramIter.value());}
    void toFront(){paramIter.toFront();}
    QHashIterator<int, CommData*> paramIter;
    virtual QByteArray prefix()=0;
    virtual QByteArray stripPrefix(const QByteArray& data)=0;
    uint8_t dataLen;
};

#endif // ABSTRACTDEVICE_H
