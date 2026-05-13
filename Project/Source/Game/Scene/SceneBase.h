#pragma once
#include "SceneType.h"

namespace MyECS { class World; class SystemManager; }

namespace MyGame {
class SceneBase {
public:
    virtual ~SceneBase() = default;

    // 遷移直前（セーブなどに利用）
    virtual void onBeforeTransition(MyECS::World& world) {}

    // シーン開始時（システム登録や初期エンティティ生成、ロードに利用）
    virtual void onEnter(MyECS::World& world, MyECS::SystemManager& systems) = 0;

    // シーン終了時（クリーンアップなど）
    virtual void onExit(MyECS::World& world) {}
};
}