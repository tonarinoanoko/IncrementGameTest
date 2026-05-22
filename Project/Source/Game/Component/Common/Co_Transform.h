#pragma once
#include <nlohmann/json.hpp>
#include "Game/Component/Common/Co_Vector2.h"

namespace MyGame {
struct Co_Transform {
    Co_Vector2 pos{};
    double scale = 1.0;
    double angle = 0.0;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Co_Transform, pos, scale, angle)
};
}