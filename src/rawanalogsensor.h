#ifndef RAWANALOGSENSOR_H
#define RAWANALOGSENSOR_H
#include "analogsensor.h"

class QSettings;

class RawAnalogSensor: public AnalogSensor
{
public:
    RawAnalogSensor(QSettings &settings);
    double value();
    bool isValueValid();
protected:
    uint16_t _maxNativeValue;
    uint16_t _minNativeValue;
private:
    double _distance;
};

#endif // RAWANALOGSENSOR_H
