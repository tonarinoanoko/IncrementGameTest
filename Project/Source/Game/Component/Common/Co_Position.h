#pragma once
#include <nlohmann/json.hpp>

namespace MyGame {
struct Co_Position {
    float x;
    float y;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Co_Position, x, y)
};
}