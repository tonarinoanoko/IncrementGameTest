#pragma once
#include <memory>
#include "SceneType.h"
#include "ECS/SystemManager.h"
#include "SceneBase.h"

namespace MyGame {
class SceneManager {
private:
    SceneType currentType;
    std::unique_ptr<SceneBase> currentSceneObj;
    std::unique_ptr<MyECS::SystemManager> currentSystems;

public:
    SceneManager();
    void changeScene(SceneType nextScene, MyECS::World& world);

    // メインループから呼ぶ
    void update(MyECS::World& world, float deltaTime);
};
}