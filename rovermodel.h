#ifndef ROVERMODEL_H
#define ROVERMODEL_H

#include <QObject>
#include "abstractdevice.h"
#include <QHash>
#include "pwmdrive.h"

class QSettings;

class RoverModel: public QObject
{
    Q_GADGET
public:
    enum RoverDevices
    {
       LeftFrontWheelDrive=1,
        LeftMiddleWheelDrive,
        LeftBackWheelDrive,
        RightFrontWheelDrive,
        RightMiddleWheelDrive,
        RightBackWheelDrive,
        LeftFrontWheelAngle,
        LeftBackWheelAngle,
        RightFrontWheelAngle,
        RightBackWheelAngle
    };
    RoverModel(QSettings& settings, QObject* parent = nullptr);
    QHash<int, AbstractDevice*> devices;
    QHashIterator<int, AbstractDevice*> deviceIter;
    bool hasNext();
    void toFront();
    QPair<int, AbstractDevice*> next();
    QPair<int, AbstractDevice *> current();
    void setRefSpeed(double speed);
    void setRefAngle(double angle);
protected:
    QString toString(RoverModel::RoverDevices devType);
    void addNewWheelDrive(QSettings& settings, RoverModel::RoverDevices devName);
    void addNewWheelAngle(QSettings& settings, RoverModel::RoverDevices devName);
    Q_ENUM(RoverDevices)
};

#endif // ROVERMODEL_H
