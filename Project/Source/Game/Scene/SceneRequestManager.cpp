#include "SceneRequestManager.h"

namespace MyGame {

SceneRequestManager::SceneRequestManager()
    : _next_scene(SceneType::Title), _requested(false)
{
}

void SceneRequestManager::request(SceneType scene)
{
    _next_scene = scene;
    _requested = true;
}

bool SceneRequestManager::hasRequest() const
{
    return _requested;
}

SceneType SceneRequestManager::popRequest()
{
    _requested = false; // 取り出すと同時にフラグを下ろす
    return _next_scene;
}

}