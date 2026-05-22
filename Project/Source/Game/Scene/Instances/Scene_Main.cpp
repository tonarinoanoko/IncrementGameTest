#include "Scene_Main.h"
#include <filesystem>

#include "ECS/World.h"
#include "ECS/SystemManager.h"

#include "Game/Systems/Common/Render/Sys_Render.h"
#include "Game/Systems/Scene_Main/Sys_MainLogic.h"
#include "Game/Systems/Common/Timer/Sys_Timer.h"
#include "Game/Systems/Scene_Main/Production/Sys_Production.h"

#include "Game/Blueprints/BP_MainResource.h"
#include "Game/Blueprints/BP_Generator.h"

namespace MyGame {
void Scene_Main::onBeforeTransition(MyECS::World& world)
{
    std::filesystem::create_directories("Save");
    world.save("autosave.json");
}

void Scene_Main::onEnter(MyECS::World& world, MyECS::SystemManager& systems)
{
    // システムの登録
    systems.addSystem<Sys_Render>();
    systems.addSystem<Sys_MainLogic>();
    systems.addSystem<Sys_Timer>();
    systems.addSystem<Sys_Production>();

    // データの復元
    /*
    if(std::filesystem::exists("Save/autosave.json")) {
        world.load("autosave.json");
    }
    */

    // BP呼び出し
    BP_MainResource::Create(world, systems);
    BP_Generator::Create(world, systems, 0);
    BP_Generator::Create(world, systems, 1);
}
}