#pragma once
#include <nlohmann/json.hpp>

namespace MyGame {
struct Co_Health {
    int hp;
    int maxHp;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Co_Health, hp, maxHp)
};
}