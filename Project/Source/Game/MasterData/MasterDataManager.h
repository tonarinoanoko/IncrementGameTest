#pragma once
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <string>
#include <nlohmann/json.hpp>
#include "Datas/MD_Base.h"
#include "Game/Global/IGlobalService.h"

namespace MyGame {
class MasterDataManager : public IGlobalService {
public:
    // JSONファイルを読み込んで特定のマスタを構築する
    template<typename T>
    void loadMaster(const std::string& path)
    {
        // ファイル読み込み処理は共通化して外部に持たせてもOK
        // ここでは簡易的に
        std::ifstream file(path);
        nlohmann::json j;
        file >> j;

        auto master = std::make_unique<T>();
        master->load(j);
        _masters[std::type_index(typeid(T))] = std::move(master);
    }

    // 取得用
    template<typename T>
    T* get()
    {
        auto it = _masters.find(std::type_index(typeid(T)));
        if(it != _masters.end()) {
            return static_cast<T*>(it->second.get());
        }
        return nullptr;
    }

    void update() override {}

private:
    std::unordered_map<std::type_index, std::unique_ptr<MD_Base>> _masters;
};
}