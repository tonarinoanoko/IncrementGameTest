#pragma once
#include <nlohmann/json.hpp>

namespace MyGame {
struct  Co_Generator
{
    double base_production = 1.0;
    int level = 0;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Co_Generator, base_production, level)
};
}
