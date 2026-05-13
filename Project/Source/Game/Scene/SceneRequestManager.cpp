#include "SceneRequestManager.h"

namespace MyGame {

SceneRequestManager::SceneRequestManager()
    : nextScene(SceneType::Title), requested(false)
{
}

void SceneRequestManager::request(SceneType scene)
{
    nextScene = scene;
    requested = true;
}

bool SceneRequestManager::hasRequest() const
{
    return requested;
}

SceneType SceneRequestManager::popRequest()
{
    requested = false; // 取り出すと同時にフラグを下ろす
    return nextScene;
}

}