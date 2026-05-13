#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace MyGame {

enum class EBuilding : int {
    factory,
    Max
};

static const std::vector<std::string> EBuildingStrings = {
    "factory",
};

inline EBuilding ToEBuilding(const std::string& idStr) {
    static const std::unordered_map<std::string, EBuilding> strToEnum = {
        { "factory", EBuilding::factory },
    };
    auto it = strToEnum.find(idStr);
    return (it != strToEnum.end()) ? it->second : EBuilding::Max;
}

inline std::string ToString(EBuilding id) {
    int index = static_cast<int>(id);
    if (index < 0 || index >= static_cast<int>(EBuilding::Max)) return "Unknown";
    return EBuildingStrings[index];
}

struct IdMax {
    static inline int EBuilding() { return static_cast<int>(EBuilding::Max); }
};

}