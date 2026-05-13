#include "Sys_LifeCycle.h"
#include "ECS/World.h"
#include "Game/Component/Common/Co_Health.h"
#include <vector>

namespace MyGame {
void Sys_LifeCycle::update(MyECS::World& world, float deltaTime)
{
    // Co_Health を持っているEntityだけを即座に取得
    auto entities = world.query<Co_Health>();

    for(auto& e : entities) {
        auto& health = world.getComponent<Co_Health>(e);
        if(health.hp <= 0) {
            world.destroyEntity(e);
        }
    }
}
}
