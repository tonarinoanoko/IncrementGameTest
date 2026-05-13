#pragma once
// ディレクトリ構造に合わせて調整
#include "ECS/ISystem.h" 

namespace MyGame {
class Sys_LifeCycle : public MyECS::ISystem {
public:
    void update(MyECS::World& world, float deltaTime) override;
};
}