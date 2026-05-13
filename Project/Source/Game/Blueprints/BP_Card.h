#pragma once
#include "ECS/Entity.h"
#include "ECS/World.h"
#include "ECS/SystemManager.h"

namespace MyGame {
class BP_Card {
public:
    // カードEntityを生成する関数
    static MyECS::Entity Create(MyECS::World& world, MyECS::SystemManager& systems, const std::string& cardId);
};
}
