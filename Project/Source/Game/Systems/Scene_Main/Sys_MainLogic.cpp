#include "Sys_MainLogic.h"
#include "Game/Global/ServiceLocator.h"
#include "Game/Input/InputManager.h"
#include "Game/Scene/SceneRequestManager.h"
#include "DxLib.h"

namespace MyGame {
void Sys_MainLogic::update(MyECS::World& world)
{
    auto input = ServiceLocator::get<InputManager>();
    auto sceneReq = ServiceLocator::get<SceneRequestManager>();

    if(input && input->isTriggered(InputAction::Decision)) {
        // 次のシーンをリクエストするだけ
        sceneReq->request(SceneType::Title);
    }
}
}