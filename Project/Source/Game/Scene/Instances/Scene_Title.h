#include "Game/Scene/SceneBase.h"

namespace MyGame {
class Scene_Title : public SceneBase {
public:
    void onEnter(MyECS::World& world, MyECS::SystemManager& systems) override;
};
}