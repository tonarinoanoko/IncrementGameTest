#pragma once
#include "MD_Base.h"
#include "Enums/ECharacter.h"
#include <nlohmann/json.hpp>
#include <unordered_map>

namespace MyGame {

struct MD_CharacterData {
    std::string id;
    std::string name;
    int value;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(MD_CharacterData, id, name, value)
};

class MD_Character : public MD_Base {
public:
    void load(const nlohmann::json& json) override {
        for(auto& [key, value] : json.items()) {
            auto data = value.get<MD_CharacterData>();
            _datas[key] = data;
        }
    }

    const MD_CharacterData* get(const std::string& id) const {
        auto it = _datas.find(id);
        return (it != _datas.end()) ? &it->second : nullptr;
    }

    const MD_CharacterData* get(ECharacter id) const {
        return get(ToString(id));
    }

private:
    std::unordered_map<std::string, MD_CharacterData> _datas;
};
}