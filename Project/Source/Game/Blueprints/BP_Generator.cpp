#include "BP_Generator.h"
#include "Game/Component/Common/Timer/Co_Timer.h"
#include "Game/Component/Generator/Co_Generator.h"


namespace MyGame {
MyECS::Entity BP_Generator::Create(MyECS::World& world, MyECS::SystemManager& systems)
{
    auto entity = world.createEntity();

    Co_Timer timer;
    timer.interval = 1.0f;
    timer.is_loop = true;
    world.addComponent<Co_Timer>(entity, timer);

    Co_Generator gene;
    gene.base_production = 1.0f;
    gene.level = 1;
    world.addComponent<Co_Generator>(entity, gene);

    return entity;
}

}