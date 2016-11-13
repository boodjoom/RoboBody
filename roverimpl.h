#ifndef ROVERIMPL_H
#define ROVERIMPL_H

#include <QObject>

class RoverImpl
{
public:
    RoverImpl();
    void setSpeed(double speed, QDateTime timeout);
private:
    double _speed;
};

#endif // ROVERIMPL_H
