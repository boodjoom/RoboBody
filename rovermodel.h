#ifndef ROVERMODEL_H
#define ROVERMODEL_H

#include "abstractdevice.h"
#include <QHash>
#include "pwmdrive.h"

class RoverModel
{
public:
    enum RoverDevices
    {
       LeftFrontWheelDrive=1,
        LeftFrontWheelRotate=2
    };
    RoverModel():deviceIter(devices)
    {
        devices[(int)LeftFrontWheelDrive] = new PwmDrive(0x0a);
    }
    QHash<int, AbstractDevice*> devices;
    QHashIterator<int, AbstractDevice*> deviceIter;
    bool hasNext(){return deviceIter.hasNext();}
    void toFront(){deviceIter.toFront();}
    QPair<int, AbstractDevice*> next(){deviceIter.next();return QPair<int,AbstractDevice*>(deviceIter.key(),deviceIter.value());}
};

#endif // ROVERMODEL_H
