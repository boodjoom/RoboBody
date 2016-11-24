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
        CurValue =2,
        SetEnabled,
        SetDisabled
    };
    enum DeviceType
    {
        DeviceType_Unknown = 0,
        DeviceType_PwmDrive,
        DeviceType_AngleController,
        DeviceType_SpeedController,
        DeviceType_RCServo
    };
    AbstractDevice();
    QHash<int, CommData*> params;
    bool hasNext();
    QPair<int, CommData*> next();
    QPair<int, CommData *> current();
    void toFront();
    QHashIterator<int, CommData*> paramIter;
    virtual QByteArray prefix()=0;
    virtual QByteArray stripPrefix(const QByteArray& data)=0;
    virtual bool isValid()=0;
    virtual AbstractDevice::DeviceType deviceType();
    uint8_t dataLen;
};

#endif // ABSTRACTDEVICE_H
