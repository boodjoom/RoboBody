#ifndef ROVER_H
#define ROVER_H

#include <QObject>
#include <QSettings>
#include "robobody_global.h"
#include "errcode.h"
class RoverImpl;

class ROBOBODYSHARED_EXPORT Rover: public QObject
{
    Q_OBJECT
public:
    Rover();
    ~Rover();
    ErrCode init(QSettings& settings);
    ErrCode deInit();
    ErrCode setSpeed(double speed);
public slots:
    void connect();
    void disconnect();
signals:
    void connected();
    void disconnected();
private:
    RoverImpl* pImpl;
};

#endif // ROVER_H
