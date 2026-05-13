#pragma once
#include "DxLib.h"
#include "Game/Global/IGlobalService.h"

namespace MyGame {
class TimeManager : public IGlobalService {
public:
    TimeManager() {};

    void update() override;

    float getDeltaTime() const { return _delta_time * _time_scale; }
    float getRawDeltaTime() const { return _delta_time; } // 倍速がかかっていない素の時間
    void setTimeScale(float scale) { _time_scale = scale; }

private:
    LONGLONG _last_time = GetNowHiPerformanceCount();
    float _delta_time = 0.0f;
    float _time_scale = 1.0f;
    double _total_game_time = 0.0;
};
}