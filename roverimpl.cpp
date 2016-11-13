#include "roverimpl.h"
#include <QDateTime>

RoverImpl::RoverImpl()
{

}

void RoverImpl::setSpeed(double speed, QDateTime timeout)
{
    if(!qFuzzyCompare(speed+1.0,_speed+1.0))
    {
        _speed=speed;
        //set model values
    }
}
