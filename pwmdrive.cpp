#include "pwmdrive.h"

PwmDrive::PwmDrive(uint8_t driveAddr):Drive(driveAddr)
{
    CommData* data = new CommData();
    data->autoUpdate=false;
    data->autoWrite=true;
    data->defaultValue=4095;
    data->readCode = READ_REFSPEED_FCN;
    data->writeCode = WRITE_REFSPEED_FCN;
    data->checkAfterWrite = false;
    data->setValue(data->defaultValue);
    data->commitChanged();
    params[(int)RefSpeed] = data;
    data = new CommData();
    data->autoUpdate=false;
    data->autoWrite=false;
    data->defaultValue=4095;
    data->readCode = READ_CURRSPEED_FCN;
    data->writeCode = 0;
    data->changed=false;
    data->checkAfterWrite = false;
    params[(int)CurSpeed] = data;
    paramIter = params;
}
