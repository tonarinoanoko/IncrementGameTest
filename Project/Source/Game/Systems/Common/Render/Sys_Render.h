#pragma once
#include "ECS/ISystem.h"

namespace MyGame {
class Sys_Render : public MyECS::ISystem {
public:
    int getPriority() const override { return MyECS::ESystemPriority::Render; }

    void update(MyECS::World& world, float deltaTime) override;
};
}