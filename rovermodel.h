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
       LeftFrontWheelRotate=2,
        LeftMiddleWheelDrive=3,
        LeftBackWheelDrive=4,
        WrightFrontWheelDrive=5,
        WrightMiddleWheelDrive=6,
        WrightBackWheelDrive=7,
        LeftBackWheelRotate,
        WrightFrontWheelRotate,
        WrightBackWheelRotate
    };
    RoverModel();
    QHash<int, AbstractDevice*> devices;
    QHashIterator<int, AbstractDevice*> deviceIter;
    bool hasNext();
    void toFront();
    QPair<int, AbstractDevice*> next();
    QPair<int, AbstractDevice *> current();
};

#endif // ROVERMODEL_H
