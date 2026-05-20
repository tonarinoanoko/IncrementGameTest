#include "Scene_Start.h"
#include "ECS/World.h"
#include "ECS/SystemManager.h"
#include "Game/Systems/Common/Render/Sys_Render.h"
#include "Game/Systems/Scene_Start/Sys_StartLogic.h"

namespace MyGame {

void Scene_Start::onEnter(MyECS::World& world, MyECS::SystemManager& systems)
{
    // システムの登録
    systems.addSystem<Sys_Render>();
    systems.addSystem<Sys_StartLogic>();
}
}