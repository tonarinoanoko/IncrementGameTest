#pragma once
#include <nlohmann/json.hpp>

namespace MyGame {
struct Co_Timer
{
    float interval = 0.0f;
    float elapsed = 0.0f;
    bool is_ready = false;
    bool is_loop = false;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Co_Timer, interval, elapsed, is_ready, is_loop)
};
}