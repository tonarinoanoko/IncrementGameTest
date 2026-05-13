#pragma once
#include "DxLib.h"
#include "Game/Global/IGlobalService.h"

namespace MyGame {
class TimeManager : public IGlobalService {
public:
    TimeManager() {};

    void update() override;

    float getDeltaTime() const { return deltaTime * timeScale; }
    float getRawDeltaTime() const { return deltaTime; } // 倍速がかかっていない素の時間
    void setTimeScale(float scale) { timeScale = scale; }

private:
    LONGLONG lastTime = GetNowHiPerformanceCount();
    float deltaTime = 0.0f;
    float timeScale = 1.0f;
    double totalGameTime = 0.0;
};
}