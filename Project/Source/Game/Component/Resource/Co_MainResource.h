#pragma once
#include <nlohmann/json.hpp>

namespace MyGame {
struct Co_MainResource
{
    double current_amount = 0.0;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Co_MainResource, current_amount)
};
}