#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace MyGame {

enum class ECharacter : int {
    Hero,
    Slime,
    Max
};

static const std::vector<std::string> ECharacterStrings = {
    "Hero",
    "Slime",
};

inline ECharacter ToECharacter(const std::string& idStr) {
    static const std::unordered_map<std::string, ECharacter> strToEnum = {
        { "Hero", ECharacter::Hero },
        { "Slime", ECharacter::Slime },
    };
    auto it = strToEnum.find(idStr);
    return (it != strToEnum.end()) ? it->second : ECharacter::Max;
}

inline std::string ToString(ECharacter id) {
    int index = static_cast<int>(id);
    if (index < 0 || index >= static_cast<int>(ECharacter::Max)) return "Unknown";
    return ECharacterStrings[index];
}

struct IdMax {
    static inline int ECharacter() { return static_cast<int>(ECharacter::Max); }
};

}