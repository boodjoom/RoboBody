#ifndef ROVERMODEL_H
#define ROVERMODEL_H

#include "abstractdevice.h"
#include <QHash>

class RoverModel
{
public:
    RoverModel():deviceIter(devices){}
    QHash<int, AbstractDevice*> devices;
    QHashIterator<int, AbstractDevice*> deviceIter;
    bool hasNext(){return deviceIter.hasNext();}
    void toFront(){deviceIter.toFront();}
    QPair<int, AbstractDevice*> next(){deviceIter.next();return QPair<int,AbstractDevice*>(deviceIter.key(),deviceIter.value());}
};

#endif // ROVERMODEL_H
