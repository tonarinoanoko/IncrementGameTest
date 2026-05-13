#include "Game/Scene/SceneBase.h"

namespace MyGame {
class Scene_Main : public SceneBase {
public:
    // メインから離れる時はオートセーブ
    void onBeforeTransition(MyECS::World& world) override;

    // メインに入る時はロードとシステム登録
    void onEnter(MyECS::World& world, MyECS::SystemManager& systems) override;
};
}