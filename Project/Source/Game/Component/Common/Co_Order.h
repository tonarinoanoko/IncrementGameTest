#pragma once
#include <nlohmann/json.hpp>

namespace MyGame {
struct Co_Order {
    int priority; // 値が大きいほど手前（後に描画）

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Co_Order, priority)
};
}