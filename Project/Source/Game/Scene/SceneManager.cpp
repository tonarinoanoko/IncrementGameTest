#include "SceneManager.h"
#include "ECS/World.h"
#include "SceneRequestManager.h"
#include "Game/Global/ServiceLocator.h"

// 各シーンの実装をインクルード
#include "Instances/Scene_Start.h"
#include "Instances/Scene_Title.h"
#include "Instances/Scene_Main.h"

namespace MyGame {

SceneManager::SceneManager()
    : _current_type(SceneType::Title), _current_scene_obj(nullptr)
{
}

void SceneManager::changeScene(SceneType next, MyECS::World& world)
{
    // --- 1. 既存シーンの終了処理 ---
    if(_current_scene_obj) {
        // 遷移の直前（セーブ等）
        _current_scene_obj->onBeforeTransition(world);
        // シーンのクリーンアップ
        _current_scene_obj->onExit(world);
    }

    // --- 2. ECS世界の初期化 ---
    world.clear();
    _current_systems = std::make_unique<MyECS::SystemManager>();

    // --- 3. 次のシーンオブジェクトの生成 ---
    _current_type = next;
    switch(next) {
    case SceneType::Start:
        _current_scene_obj = std::make_unique<Scene_Start>();
        break;
    case SceneType::Title:
        _current_scene_obj = std::make_unique<Scene_Title>();
        break;
    case SceneType::Main:
        _current_scene_obj = std::make_unique<Scene_Main>();
        break;
    }

    // --- 4. 新しいシーンの開始処理 ---
    if(_current_scene_obj) {
        // システムの登録やロード、初期Entityの生成を行う
        _current_scene_obj->onEnter(world, *_current_systems);
    }
}

void SceneManager::update(MyECS::World& world)
{
    if(_current_systems) {
        _current_systems->updateAll(world);
    }

    auto sceneReq = ServiceLocator::get<SceneRequestManager>();
    if(sceneReq && sceneReq->hasRequest()) {
        this->changeScene(sceneReq->popRequest(), world);
    }
}
}