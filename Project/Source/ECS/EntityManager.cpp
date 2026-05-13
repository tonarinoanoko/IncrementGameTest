#include "EntityManager.h"
#include <algorithm>

namespace MyECS {
EntityManager::EntityManager() {}

Entity EntityManager::create()
{
    if(!_free_entities.empty()) {
        uint32_t id = _free_entities.back();
        _free_entities.pop_back();
        // masks は既に確保済み
        _masks[id].reset(); // マスクをリセット
        // generation は destroy() 時にインクリメント済みなので、その値を使う
        return Entity{ id, _generations[id] };
    }
    uint32_t id = _next_id++;
    _masks.emplace_back(); // 新しいID用のマスク領域を確保
    _generations.push_back(0);
    return Entity{ id, 0 };
}

void EntityManager::destroy(Entity e)
{
    if(e.id >= _generations.size()) return; // 安全策

    // 世代をインクリメントして古いハンドルを無効化
    ++_generations[e.id];
    _masks[e.id].reset(); // 破壊されたらコンポーネント情報を消す
    _free_entities.push_back(e.id);
}

void EntityManager::clear()
{
    _next_id = 0;
    _free_entities.clear();
    _masks.clear();
    _generations.clear();
}

ComponentMask EntityManager::getMask(Entity e) const
{
    return _masks[e.id];
}

void EntityManager::setMask(Entity e, ComponentMask mask)
{
    _masks[e.id] = mask;
}

bool EntityManager::isAlive(Entity e) const
{
    // 世代管理により高速判定
    return e.id < static_cast<uint32_t>(_generations.size()) && _generations[e.id] == e.generation;
}

std::vector<Entity> EntityManager::getLivingEntities() const
{
    std::vector<Entity> living;

    // generations のサイズ（現在の最大ID数）をループ
    for(uint32_t i = 0; i < static_cast<uint32_t>(_generations.size()); ++i) {
        Entity e{i, _generations[i]};
        if(isAlive(e)) {
            living.push_back(e);
        }
    }
    return living;
}
}
