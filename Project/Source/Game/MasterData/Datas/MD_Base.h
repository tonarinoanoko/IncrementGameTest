#pragma once
#include <nlohmann/json.hpp>

namespace MyGame {
// 全マスタデータの基底
class MD_Base {
public:
    virtual ~MD_Base() = default;
    virtual void load(const nlohmann::json& json) = 0;
};
}