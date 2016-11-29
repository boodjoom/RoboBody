#include "angledrive.h"

AngleDrive::AngleDrive(uint8_t addr)
    : Drive(addr)
{
    CommData* data = new CommData();
    data->autoUpdate=false;
    data->autoWrite=false;
    data->defaultValue=0;
    data->readCode = READ_ANGLE_FCN;
    data->writeCode = WRITE_ANGLE_FCN;
    data->checkAfterWrite = false;
    data->setValue(data->defaultValue);
    data->commitChanged();
    params[(int)RefAngle] = data;
    data = new CommData();
    data->autoUpdate=false;
    data->autoWrite=false;
    data->defaultValue=7777;
    data->readCode = READ_ADC_FCN;
    data->writeCode = 0;
    data->changed=false;
    data->checkAfterWrite = false;
    params[(int)CurAngle] = data;
    paramIter = params;
}

AbstractDevice::DeviceType AngleDrive::deviceType()
{
    return AbstractDevice::DeviceType_AngleController;
}
