#ifndef ROVER_H
#define ROVER_H

#include <QObject>
#include <QSettings>
#include "robobody_global.h"
#include "errcode.h"
#include "warcode.h"
#include "manipstate.h"

class RoverImpl;

class ROBOBODYSHARED_EXPORT Rover: public QObject
{
    Q_OBJECT
public:
    Rover();
    ~Rover();
    ErrCode init(QSettings& settings);
    ErrCode deInit();
    ErrCode setRefSpeed(double speed, unsigned long long timeout = 0);
    double getCurSpeed(ErrCode* err = nullptr);
    double getRefSpeed(ErrCode* err = nullptr);
    ErrCode setBreaksEnabled(bool enabled);
    bool isBreaksEnabled(ErrCode* err = nullptr);
    ErrCode setRefYaw(double yaw);
    double getRefYaw(ErrCode* err = nullptr);
    double getCurYaw(ErrCode* err = nullptr);
    double getTravel(ErrCode* err = nullptr);
    void startManip();
    ManipState getManipState();
    Q_ENUM(ManipState)
    static QString toString(ManipState state);
public slots:
    void connect();
    void disconnect();
signals:
    void connected();
    void disconnected();
    void error(ErrCode);
    void warning(WarCode);
private:
    RoverImpl* pImpl;
};

#endif // ROVER_H
