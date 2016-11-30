#ifndef ROVERIMPL_H
#define ROVERIMPL_H

#include <QObject>
#include <QSettings>
#include "errcode.h"
#include "warcode.h"
#include <QElapsedTimer>
#include <QTimer>
#include "manipstate.h"
#include "manipaction.h"
#include "battery.h"

class RoverModel;
class SerialComm;

class RoverImpl: public QObject
{
    Q_OBJECT
public:
    RoverImpl();
    ErrCode init(QSettings &settings);
    void setRefSpeed(double speed, quint64 timeout);
    void setRefYaw(double yaw, quint64 timeout);
    void deInit();
    void openPort();
    void closePort();
    ErrCode setBreaks(bool enabled);
    double getRefSpeed(ErrCode* err = nullptr);
    double getRefYaw(ErrCode* err = nullptr);
    double getTravel(ErrCode* err = nullptr);
    void startManip();
    ManipState getManipState();
    enum class ManipStage{StartManip,MoveToTarget,OpenGripper,CloseGripper, MoveToHome, StopManip};
    Q_ENUM(ManipStage)
    double getBattery(Battery battery, ErrCode* err = nullptr);
    void execManip(ManipAction action);
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
    double _yaw;
    SerialComm* _serialComm;
    QThread* _commThread;
    RoverModel* _roverModel;
    QTimer* _refSpeedTimer;
private slots:
    void onRefSpeedTimeout();
    void onManipStageTimeout();
private:
    double _distanceApproximation;
    QElapsedTimer _speedOnTimer;
    double approximateDistance(double newSpeedOnInterval);
    double _intervalCorrection;
    double _speedApproximation;
    QTimer _manipStageTimer;
    ManipStage _curManipStage;
    QString toString(RoverImpl::ManipStage stage);
    void setCurManipStage(ManipStage stage);
};

#endif // ROVERIMPL_H
