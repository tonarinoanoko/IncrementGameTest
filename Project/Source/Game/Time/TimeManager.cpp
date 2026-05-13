#include "TimeManager.h"

namespace MyGame {
void TimeManager::update()
{
    LONGLONG currentTime = GetNowHiPerformanceCount();
    // 秒単位で算出
    deltaTime = static_cast<float>(currentTime - lastTime) / 1000000.0f;
    lastTime = currentTime;

    // ゲーム内累計時間（倍速などの影響を受ける用）
    totalGameTime += deltaTime * timeScale;
}
}