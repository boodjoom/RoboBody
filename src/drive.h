#ifndef DRIVE_H
#define DRIVE_H
#include <QDebug>
#include "abstractdevice.h"

class Drive: public AbstractDevice
{
public:
    Drive(uint8_t driveAddr);
    uint8_t addr;
    virtual QByteArray prefix();
    virtual QByteArray stripPrefix(const QByteArray& data);
    virtual bool isValid();
};

#endif // DRIVE_H
