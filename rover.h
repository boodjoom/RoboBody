#ifndef ROVER_H
#define ROVER_H

#include "robobody_global.h"
class RoverImpl;

class ROBOBODYSHARED_EXPORT Rover
{

public:
    Rover();
    ~Rover();
private:
    RoverImpl* pImpl;
};

#endif // ROVER_H
