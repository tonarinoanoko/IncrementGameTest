#include "Scene_Title.h"
#include "ECS/World.h"
#include "ECS/SystemManager.h"
#include "Game/Systems/Common/Render/Sys_Render.h"
#include "Game/Systems/Scene_Title/Sys_TitleLogic.h"

namespace MyGame {

void Scene_Title::onEnter(MyECS::World& world, MyECS::SystemManager& systems)
{
    // システムの登録
    systems.addSystem<Sys_Render>();
    systems.addSystem<Sys_TitleLogic>();
}
}