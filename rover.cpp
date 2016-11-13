#include "rover.h"
#include "roverimpl.h"

Rover::Rover()
{
    pImpl = new RoverImpl();
}

Rover::~Rover()
{
    delete pImpl;
}
