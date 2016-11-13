#include "rovermodel.h"

RoverModel::RoverModel():deviceIter(devices)
{
    devices[(int)LeftFrontWheelDrive] = new PwmDrive(0x0a);
    deviceIter = devices;
}

bool RoverModel::hasNext(){return deviceIter.hasNext();}

void RoverModel::toFront(){deviceIter.toFront();}

QPair<int, AbstractDevice *> RoverModel::next(){deviceIter.next();return QPair<int,AbstractDevice*>(deviceIter.key(),deviceIter.value());}
