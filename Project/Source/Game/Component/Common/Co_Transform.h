#pragma once
#include <nlohmann/json.hpp>

namespace MyGame {
struct Co_Transform {
    float x, y;
    float scale;
    float angle;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Co_Transform, x, y, scale, angle)
};
}