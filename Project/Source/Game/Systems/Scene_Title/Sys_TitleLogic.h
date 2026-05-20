#pragma once
#include "ECS/ISystem.h"

namespace MyGame {
class Sys_TitleLogic : public MyECS::ISystem {
public:
    void update(MyECS::World& world) override;
};
}