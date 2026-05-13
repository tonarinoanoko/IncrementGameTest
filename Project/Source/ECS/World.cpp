#include "World.h"
#include <fstream>
#include <iomanip>

namespace MyECS {

void World::clear()
{
    // 全てのコンポーネントデータを削除
    for(auto& pair : _component_pools) {
        pair.second->clear();
    }
    // エンティティ管理のリセット
    _entity_manager.clear();
}

void World::save(const std::string& filename)
{
    nlohmann::json root;

    // 1. 基本情報の保存
    root["nextID"] = _entity_manager.getCapacity();

    // 2. 各コンポーネントプールのシリアライズ
    // コンポーネントが存在しないプールは保存をスキップしても良い
    for(auto& [typeName, pool] : _component_pools) {
        root["components"][typeName] = pool->serialize();
    }

    // 3. ファイル書き出し
    std::string path = "Save/" + filename;
    std::ofstream os(path);
    if(os.is_open()) {
        os << std::setw(4) << root << std::endl;
    }
}

void World::load(const std::string& filename)
{
    std::string path = "Save/" + filename;
    std::ifstream is(path);
    if(!is.is_open()) return;

    nlohmann::json root;
    is >> root;

    this->clear();

    if(root.contains("components")) {
        // 1. 各プールにデータを流し込む
        for(auto it = root["components"].begin(); it != root["components"].end(); ++it) {
            std::string typeName = it.key();
            if(_component_pools.count(typeName)) {
                _component_pools[typeName]->deserialize(it.value());
            }
        }

        // 2. 重要：全Entityのビットマスクを再構築
        for(auto& [typeName, pool] : _component_pools) {
            uint32_t tid = _component_registry.getIDByTypeName(typeName);

            // プール内にデータがある全Entityを取得（前回追加した関数）
            auto entities = pool->getAliveEntities();
            for(auto& e : entities) {
                ComponentMask mask = _entity_manager.getMask(e);
                mask.set(tid);
                _entity_manager.setMask(e, mask);
            }
        }
    }
}
}