#include "rovermodel.h"
#include <QSettings>
#include "roverwheeldrive.h"
#include "roverwheelangle.h"
#include "manipangle.h"
#include "twoposercservo.h"
#include "rawanalogsensor.h"
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
    addNewWheelAngle(settings,RightBackWheelAngle);
    addNewManipAngle(settings, FirstManipAngle);
    addNewManipAngle(settings, SecondManipAngle);
    calibManip();
    addNewTwoPoseRCServo(settings, ManipGripper);
    setManipGripperPose(GripperPose::Opened);//default opened
    addNewRawAnalogSensor(settings,BodyBatterySensor);
    addNewRawAnalogSensor(settings,BrainBatterySensor);
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

void RoverModel::setManipPose(ManipPose newPose)
{
    if(newPose == ManipPose::Base)
    {
        calibManip();
        return;
    }
    QHashIterator<int, AbstractDevice*> it(devices);
    it.toFront();
    while(it.hasNext())
    {
        ManipAngle* manipAngle = dynamic_cast<ManipAngle*>(it.next().value());
        if(manipAngle)
        {
            switch(newPose)
            {
            case ManipPose::Home: manipAngle->moveToHome();break;
            case ManipPose::Target: manipAngle->moveToTarget();break;
            default:
                break;
            };
        }
    }
}

ManipState RoverModel::getManipState()
{
    ManipState result = ManipState::Unknown;
    ManipAngle* firstDrive = dynamic_cast<ManipAngle*>(devices[(int)FirstManipAngle]);
    ManipAngle* secondDrive = dynamic_cast<ManipAngle*>(devices[(int)SecondManipAngle]);
    if(firstDrive && secondDrive)
    {
        const ManipState first = firstDrive->getState();
        const ManipState second = secondDrive->getState();
        //if any is moving all is moving
        if(first == ManipState::Moving || second == ManipState::Moving)
            result=ManipState::Moving;
        else if(first == second)
                result=first;
        //else Unknown
    }
    return result;
}

void RoverModel::setManipGripperPose(GripperPose newPose)
{
    TwoPoseRCServo* gripper = dynamic_cast<TwoPoseRCServo*>(devices[(int)ManipGripper]);
    if(gripper)
    {
        switch(newPose)
        {
        case GripperPose::Opened: gripper->open();break;
        case GripperPose::Closed: gripper->close();break;
        default:break;
        };
    }
}

GripperState RoverModel::getManipGripperState()
{
    GripperState result = GripperState::Unknown;
    TwoPoseRCServo* gripper = dynamic_cast<TwoPoseRCServo*>(devices[(int)ManipGripper]);
    if(gripper)
    {
        result = gripper->getState();
    }
    return result;
}

double RoverModel::getSensor(RoverDevices sensorDevice)
{
    double result =0.0;
    RawAnalogSensor* sensor = dynamic_cast<RawAnalogSensor*>(devices[(int)sensorDevice]);
    if(sensor)
    {
        result = sensor->value();
    }
    return result;
}

void RoverModel::calibManip()
{
    //enable first to start calib
    ManipAngle* firstDrive = dynamic_cast<ManipAngle*>(devices[(int)FirstManipAngle]);
    firstDrive->params[(int)AbstractDevice::SetEnabled]->changed=true;
    firstDrive->moveToBase();
    //disable second to start calib
    ManipAngle* secondDrive = dynamic_cast<ManipAngle*>(devices[(int)SecondManipAngle]);
    secondDrive->params[(int)AbstractDevice::SetDisabled]->changed=true;
    secondDrive->moveToBase();
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

void RoverModel::addNewManipAngle(QSettings &settings, RoverModel::RoverDevices devName)
{
    QString group = toString(devName);
    qDebug()<<"reading group "<<group<<" from settings";
    settings.beginGroup(group);
    devices[(int)devName] = new ManipAngle(settings);
    settings.endGroup();
}

void RoverModel::addNewTwoPoseRCServo(QSettings &settings, RoverModel::RoverDevices devName)
{
    QString group = toString(devName);
    qDebug()<<"reading group "<<group<<" from settings";
    settings.beginGroup(group);
    devices[(int)devName] = new TwoPoseRCServo(settings);
    settings.endGroup();
}

void RoverModel::addNewRawAnalogSensor(QSettings &settings, RoverModel::RoverDevices devName)
{
    QString group = toString(devName);
    qDebug()<<"reading group "<<group<<" from settings";
    settings.beginGroup(group);
    devices[(int)devName] = new RawAnalogSensor(settings);
    settings.endGroup();
}

