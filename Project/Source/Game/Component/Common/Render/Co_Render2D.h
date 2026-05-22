#pragma once
#include <nlohmann/json.hpp>
#include "Game/Component/Common/Co_Vector2.h"
#include "Game/Component/Common/Co_Transform.h"
#include "Game/Component/Common/Co_Color.h"

namespace MyGame {

enum class ERender2D_Type : uint8_t {
    Sprite,
    Box,
    Text,
};

struct Render2D_Element
{
    ERender2D_Type type{};
    Co_Transform transform{};

    int handle = -1; // DxLibのグラフィックハンドル
    Co_Vector2 size{};
    std::string text{};

    Co_Color color {255, 255, 255};

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Render2D_Element, type, transform, handle, size, text, color)

    void setTextValue(float value, int precision)
    {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(precision) << value;
        text = ss.str();
    }

    void setTextValue(double value, int precision)
    {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(precision) << value;
        text = ss.str();
    }
};

struct Co_Render2D {
    Co_Vector2 base_pos{};
    int priorty = 0;
    bool is_visble = true;
    std::vector<Render2D_Element> elements{};

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Co_Render2D, base_pos, priorty, is_visble, elements)
};
}
