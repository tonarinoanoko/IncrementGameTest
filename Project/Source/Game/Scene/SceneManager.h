#pragma once
#include <memory>
#include "SceneType.h"
#include "ECS/SystemManager.h"
#include "SceneBase.h"

namespace MyGame {
class SceneManager {
public:
    SceneManager();
    void changeScene(SceneType nextScene, MyECS::World& world);

    // メインループから呼ぶ
    void update(MyECS::World& world, float deltaTime);

private:
    SceneType _current_type;
    std::unique_ptr<SceneBase> _current_scene_obj;
    std::unique_ptr<MyECS::SystemManager> _current_systems;
};
}