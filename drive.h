#ifndef DRIVE_H
#define DRIVE_H

#include "abstractdevice.h"

class Drive: public AbstractDevice
{
public:
    Drive(uint8_t driveAddr):AbstractDevice(),addr(driveAddr)
    {
        dataLen = 4;//pack - crc

    }
    uint8_t addr;
    virtual QByteArray prefix()
    {
        char buf[] = {(char)addr};
        return QByteArray(buf,1);
    }
    virtual QByteArray stripPrefix(const QByteArray& data)
    {
        return data.right(data.length()-1);
    }
};

#endif // DRIVE_H
