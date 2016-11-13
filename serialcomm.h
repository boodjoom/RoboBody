#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include "stdint.h"
#include <QObject>

#include "errcode.h"

class RoverModel;

class SerialComm : public QObject
{
    Q_OBJECT
public:
    explicit SerialComm(QObject *parent = 0);

signals:

public slots:
protected:
    void run();
    RoverModel* model;
    ErrCode write(QByteArray data);
    void addCrc(QByteArray& data);
    bool stopFlag;
    QByteArray read(QByteArray req, uint8_t dataBytesToRead, ErrCode* err = nullptr);
};

#endif // SERIALCOMM_H
