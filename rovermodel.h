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
    RoverModel();
    QHash<int, AbstractDevice*> devices;
    QHashIterator<int, AbstractDevice*> deviceIter;
    bool hasNext();
    void toFront();
    QPair<int, AbstractDevice*> next();
};

#endif // ROVERMODEL_H
