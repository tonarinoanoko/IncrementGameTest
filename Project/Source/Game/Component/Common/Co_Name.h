#pragma once
#include <string>
#include <nlohmann/json.hpp>

namespace MyGame {
struct Co_Name {
    std::string name;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Co_Name, name)
};
}