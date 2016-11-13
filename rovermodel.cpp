#include "rovermodel.h"

RoverModel::RoverModel():deviceIter(devices)
{
    devices[(int)LeftFrontWheelDrive] = new PwmDrive(0x0a);
    devices[(int)LeftMiddleWheelDrive] = new PwmDrive(0x0b);
    devices[(int)LeftBackWheelDrive] = new PwmDrive(0x0c);
    devices[(int)WrightFrontWheelDrive] = new PwmDrive(0x0d);
    devices[(int)WrightMiddleWheelDrive] = new PwmDrive(0x0e);
    devices[(int)WrightBackWheelDrive] = new PwmDrive(0x0f);
    deviceIter = devices;
}

bool RoverModel::hasNext(){return deviceIter.hasNext();}

void RoverModel::toFront(){deviceIter.toFront();}

QPair<int, AbstractDevice *> RoverModel::next(){deviceIter.next();return QPair<int,AbstractDevice*>(deviceIter.key(),deviceIter.value());}
