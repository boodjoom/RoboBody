#ifndef ROVERIMPL_H
#define ROVERIMPL_H

#include <QObject>
#include <QSettings>
#include "errcode.h"
#include "warcode.h"

class RoverModel;
class SerialComm;

class RoverImpl: public QObject
{
    Q_OBJECT
public:
    RoverImpl();
    ErrCode init(QSettings &settings);
    void setSpeed(double speed, QDateTime timeout);
    void deInit();
    void openPort();
    void closePort();
signals:
    void open();
    void close();
    void start();
    void stop();
    void opened();
    void closed();
    void error(ErrCode);
    void warning(WarCode);
private:
    double _speed;
    SerialComm* _serialComm;
    QThread* _commThread;
    RoverModel* _roverModel;
};

#endif // ROVERIMPL_H
