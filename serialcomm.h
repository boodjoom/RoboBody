#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include "stdint.h"
#include <QObject>
#include <QtSerialPort/QSerialPort>

#include "errcode.h"
#include "warcode.h"

class RoverModel;

class SerialComm : public QObject
{
    Q_OBJECT
public:
    explicit SerialComm(QObject *parent = 0);
    void setModel(RoverModel* m){model=m;}
    void setPortName(const QString& name){portName=name;}
    void setBoudRate(int rate){boudRate=rate;}
signals:
    void finished();
    void opened();
    void closed();
    void error(ErrCode code);
    void warning(WarCode code);
public slots:
    void open();
    void close();
    void stop();
    void start();
protected:
    void run();
    RoverModel* model;
    ErrCode write(QByteArray data);
    void addCrc(QByteArray& data);
    bool stopFlag;
    QByteArray read(QByteArray req, uint8_t dataBytesToRead, ErrCode* err = nullptr);
    QString portName;
    int boudRate;
    QSerialPort* _port;
    bool checkCrc(QByteArray& data);
    void removeLeadingZeros(QByteArray &data);
};

#endif // SERIALCOMM_H
