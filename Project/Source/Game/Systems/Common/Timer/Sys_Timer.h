#pragma once
#include "ECS/ISystem.h" 

namespace MyGame {
class Sys_Timer : public MyECS::ISystem
{
public:
    void update(MyECS::World& world) override;
};
}
