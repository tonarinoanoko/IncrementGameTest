#pragma once
#include <nlohmann/json.hpp>

namespace MyGame {
struct Co_Timer
{
    float interval;
    float elapsed;
    bool is_ready;
    bool is_loop;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Co_Timer, interval, elapsed, is_ready, is_loop)
};
}