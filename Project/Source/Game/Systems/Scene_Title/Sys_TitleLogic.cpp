#include "Sys_TitleLogic.h"
#include "Game/Global/ServiceLocator.h"
#include "Game/Input/InputManager.h"
#include "Game/Scene/SceneRequestManager.h"
#include "DxLib.h"

namespace MyGame {
void Sys_TitleLogic::update(MyECS::World& world)
{
    auto input = ServiceLocator::get<InputManager>();
    auto sceneReq = ServiceLocator::get<SceneRequestManager>();

    DrawString(100, 100, "TITLE SCENE: PRESS SPACE TO START", GetColor(255, 255, 255));

    if(input && input->isTriggered(InputAction::Decision)) {
        // 次のシーンをリクエストするだけ
        sceneReq->request(SceneType::Main);
    }
}
}