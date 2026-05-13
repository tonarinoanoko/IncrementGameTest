#pragma once
#include <string>
#include <nlohmann/json.hpp>

namespace MyGame {
struct Co_Text {
    std::string content;
    unsigned int color;
    int fontSize;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Co_Text, content, color, fontSize)
};
}