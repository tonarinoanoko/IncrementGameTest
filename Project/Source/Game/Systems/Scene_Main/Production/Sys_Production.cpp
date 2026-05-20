#include "Sys_Production.h"
#include "DxLib.h"
#include "ECS/World.h"
#include "Game/Component/Common/Timer/Co_Timer.h"
#include "Game/Component/Resource/Co_MainResource.h"
#include "Game/Component/Generator/Co_Generator.h"

namespace MyGame {

void Sys_Production::update(MyECS::World& world)
{
    auto main_entities = world.query<Co_MainResource>();
    auto gene_entities = world.query<Co_Generator, Co_Timer>();

    for(auto& entity : main_entities) {
        auto& resource = world.getComponent<Co_MainResource>(entity);

        for(auto gene_entitiy : gene_entities) {
            auto& timer = world.getComponent<Co_Timer>(gene_entitiy);
            if(timer.is_ready == false) {
                continue;
            }

            auto& gene = world.getComponent<Co_Generator>(gene_entitiy);
            resource.current_amount += gene.base_production * gene.level;

            timer.is_ready = false;
        }

        DrawFormatString(10,10, GetColor(255, 255, 255), "resource %.0f", resource.current_amount);
    }
}

}
