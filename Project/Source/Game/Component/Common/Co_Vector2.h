#pragma once
#include <nlohmann/json.hpp>

namespace MyGame {
struct Co_Vector2
{
    float x = 0.0f;
    float y = 0.0f;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Co_Vector2, x, y)

    void set(float set_x, float set_y)
    {
        x = set_x;
        y = set_y;
    }

    int getIntX()
    {
        return static_cast<int>(x);
    }

    int getIntY()
    {
        return static_cast<int>(y);
    }
};
}