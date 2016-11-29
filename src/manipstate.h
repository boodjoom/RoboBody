#ifndef MANIPSTATE_H
#define MANIPSTATE_H

enum class ManipState{Unknown,Idle,Moving,AtBase,AtTarget,AtHome};
enum class ManipPose
{
    Base,
    Home,
    Target
};

#endif // MANIPSTATE_H
