#pragma once
#include "SceneType.h"
#include "Game/Global/IGlobalService.h"

namespace MyGame {
class SceneRequestManager : public IGlobalService {
private:
    SceneType nextScene;
    bool requested;

public:
    SceneRequestManager();
    void update() override {} // 特に毎フレームやることはない

    void request(SceneType scene);
    bool hasRequest() const;
    SceneType popRequest(); // リクエストを取り出し、フラグを折る
};
}