#include "rovermodel.h"
#include <QSettings>
#include "roverwheeldrive.h"
#include "roverwheelangle.h"
#include <QMetaEnum>

RoverModel::RoverModel(QSettings &settings, QObject *parent)
    : QObject(parent)
    , deviceIter(devices)
{
    addNewWheelDrive(settings,LeftFrontWheelDrive);
    addNewWheelDrive(settings,LeftMiddleWheelDrive);
    addNewWheelDrive(settings,LeftBackWheelDrive);
    addNewWheelDrive(settings,RightFrontWheelDrive);
    addNewWheelDrive(settings,RightMiddleWheelDrive);
    addNewWheelDrive(settings,RightBackWheelDrive);
    addNewWheelAngle(settings,LeftFrontWheelAngle);
    addNewWheelAngle(settings,LeftBackWheelAngle);
    addNewWheelAngle(settings,RightFrontWheelAngle);
    addNewWheelAngle(settings,RightFrontWheelAngle);
    deviceIter = devices;
}

bool RoverModel::hasNext(){return deviceIter.hasNext();}

void RoverModel::toFront(){deviceIter.toFront();}

QPair<int, AbstractDevice *> RoverModel::next(){deviceIter.next();return QPair<int,AbstractDevice*>(deviceIter.key(),deviceIter.value());}

QPair<int, AbstractDevice *> RoverModel::current()
{return QPair<int,AbstractDevice*>(deviceIter.key(),deviceIter.value());}

void RoverModel::setRefSpeed(double speed)
{
    //for each speed devices
    QHashIterator<int, AbstractDevice*> it(devices);
    it.toFront();
    while(it.hasNext())
    {
        AbstractDevice* device = it.next().value();
        if(device->deviceType() == AbstractDevice::DeviceType_PwmDrive)
        {
            RoverWheelDrive* drive = dynamic_cast<RoverWheelDrive*>(device);
            if(drive)
                drive->setRefSpeed(speed);
        }
    }
}

void RoverModel::setRefAngle(double angle)
{
    //for each angle devices
    QHashIterator<int, AbstractDevice*> it(devices);
    it.toFront();
    while(it.hasNext())
    {
        RoverWheelAngle* drive = dynamic_cast<RoverWheelAngle*>(it.next().value());
        if(drive)
            drive->setRefAngle(angle);
    }
}

QString RoverModel::toString(RoverModel::RoverDevices devType)
{
    static int enumIdx = RoverModel::staticMetaObject.indexOfEnumerator("RoverDevices");
    return RoverModel::staticMetaObject.enumerator(enumIdx).valueToKey(devType);
}

void RoverModel::addNewWheelDrive(QSettings &settings, RoverModel::RoverDevices devName)
{
    QString group = toString(devName);
    qDebug()<<"reading group "<<group<<" from settings";
    settings.beginGroup(group);
    devices[(int)devName] = new RoverWheelDrive(settings);
    settings.endGroup();
}

void RoverModel::addNewWheelAngle(QSettings &settings, RoverModel::RoverDevices devName)
{
    QString group = toString(devName);
    qDebug()<<"reading group "<<group<<" from settings";
    settings.beginGroup(group);
    devices[(int)devName] = new RoverWheelAngle(settings);
    settings.endGroup();
}

