#pragma once
#include <nlohmann/json.hpp>
#include "DxLib.h"

namespace MyGame {

enum class EColorType : uint8_t {
    White,
    Black,
};

struct Co_Color
{
    int r = 0;
    int g = 0;
    int b = 0;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Co_Color, r, g, b)

    void set(int set_r, int set_g, int set_b)
    {
        r = set_r;
        g = set_g;
        b = set_b;
    }

    void set(EColorType color)
    {
        switch(color) {
        case MyGame::EColorType::White:
            set(255, 255, 255);
            break;
        case MyGame::EColorType::Black:
            set(0, 0, 0);
            break;
        }
    }

    unsigned int getColor()
    {
        return GetColor(r, g, b);
    }
};

}
