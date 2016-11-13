#ifndef ROVERIMPL_H
#define ROVERIMPL_H

#include <QObject>
#include <QSettings>

class RoverModel;
class SerialComm;

class RoverImpl: public QObject
{
    Q_OBJECT
public:
    RoverImpl();
    void init(QSettings &settings);
    void setSpeed(double speed, QDateTime timeout);
    void deInit();
signals:
    void open();
    void close();
    void start();
    void stop();
private:
    double _speed;
    SerialComm* _serialComm;
    QThread* _commThread;
    RoverModel* _roverModel;
};

#endif // ROVERIMPL_H
