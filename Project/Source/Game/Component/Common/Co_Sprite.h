#pragma once
#include <nlohmann/json.hpp>

namespace MyGame {
struct Co_Sprite {
    int handle;       // DxLibのグラフィックハンドル
    int priority;     // 描画順（Z値）
    bool isVisible;   // 表示フラグ

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Co_Sprite, handle, priority, isVisible)
};
}