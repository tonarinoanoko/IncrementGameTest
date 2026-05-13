#pragma once
#include "ECS/ISystem.h"

namespace MyGame {
class Sys_MainLogic : public MyECS::ISystem {
public:
    void update(MyECS::World& world, float deltaTime) override;
};
}