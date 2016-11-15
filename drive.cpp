#include "drive.h"

Drive::Drive(uint8_t driveAddr):AbstractDevice(),addr(driveAddr)
{
    dataLen = 4;//pack - crc
}

QByteArray Drive::prefix()
{
    //qDebug()<<"drive prefix";
    char buf[] = {(char)addr};
    return QByteArray(buf,1);
}

QString toString2(const QByteArray& data)
{
    QString str;
    for(int i=0;i<data.size();++i)
        str+=QString("%1 ").arg((uint8_t)data[i],2,16);
    return str;
}

QByteArray Drive::stripPrefix(const QByteArray &data)
{
    //qDebug()<<"drive stripPrefix data:"<<toString2(data);
    if(data.isEmpty())
        return data;
    else
        return data.right(data.length()-1);
}

bool Drive::isValid()
{
    return (addr != 0);
}
