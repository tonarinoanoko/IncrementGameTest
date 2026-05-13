#include "SceneManager.h"
#include "ECS/World.h"
#include "SceneRequestManager.h"
#include "Game/Global/ServiceLocator.h"

// 各シーンの実装をインクルード
#include "Instances/Scene_Title.h"
#include "Instances/Scene_Main.h"
#include "Instances/Scene_Battle.h"

namespace MyGame {

SceneManager::SceneManager()
    : currentType(SceneType::Title), currentSceneObj(nullptr)
{
}

void SceneManager::changeScene(SceneType nextType, MyECS::World& world)
{
    // --- 1. 既存シーンの終了処理 ---
    if(currentSceneObj) {
        // 遷移の直前（セーブ等）
        currentSceneObj->onBeforeTransition(world);
        // シーンのクリーンアップ
        currentSceneObj->onExit(world);
    }

    // --- 2. ECS世界の初期化 ---
    world.clear();
    currentSystems = std::make_unique<MyECS::SystemManager>();

    // --- 3. 次のシーンオブジェクトの生成 ---
    currentType = nextType;
    switch(nextType) {
    case SceneType::Title:
        currentSceneObj = std::make_unique<Scene_Title>();
        break;
    case SceneType::Main:
        currentSceneObj = std::make_unique<Scene_Main>();
        break;
    case SceneType::Battle:
        currentSceneObj = std::make_unique<Scene_Battle>();
        break;
    }

    // --- 4. 新しいシーンの開始処理 ---
    if(currentSceneObj) {
        // システムの登録やロード、初期Entityの生成を行う
        currentSceneObj->onEnter(world, *currentSystems);
    }
}

void SceneManager::update(MyECS::World& world, float deltaTime)
{
    if(currentSystems) {
        currentSystems->updateAll(world, deltaTime);
    }

    auto sceneReq = ServiceLocator::get<SceneRequestManager>();
    if(sceneReq && sceneReq->hasRequest()) {
        this->changeScene(sceneReq->popRequest(), world);
    }
}
}