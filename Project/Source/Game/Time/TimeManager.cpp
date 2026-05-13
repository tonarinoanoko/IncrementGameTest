#include "TimeManager.h"

namespace MyGame {
void TimeManager::update()
{
    LONGLONG current_time = GetNowHiPerformanceCount();
    // 秒単位で算出
    _delta_time = static_cast<float>(current_time - _last_time) / 1000000.0f;
    _last_time = current_time;

    // ゲーム内累計時間（倍速などの影響を受ける用）
    _total_game_time += _delta_time * _time_scale;
}
}