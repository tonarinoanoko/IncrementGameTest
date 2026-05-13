#include "Scene_Main.h"
#include "ECS/World.h"
#include "ECS/SystemManager.h"
#include "Game/Systems/Common/Render/Sys_Render.h"
#include "Game/Systems/Scene_Main/Sys_MainLogic.h"
#include <filesystem>

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

    // データの復元
    if(std::filesystem::exists("Save/autosave.json")) {
        world.load("autosave.json");
    }
}
}