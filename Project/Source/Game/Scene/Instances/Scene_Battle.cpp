#include "Scene_Battle.h"
#include "ECS/World.h"
#include "ECS/SystemManager.h"
#include "Game/Systems/Common/Render/Sys_Render.h"
#include <filesystem>

namespace MyGame {

void Scene_Battle::onEnter(MyECS::World& world, MyECS::SystemManager& systems)
{
    // システムの登録
    systems.addSystem<Sys_Render>();
}
}