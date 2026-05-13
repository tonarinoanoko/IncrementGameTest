#include "EntityManager.h"
#include <algorithm>

namespace MyECS {
EntityManager::EntityManager() {}

Entity EntityManager::create()
{
    if(!freeEntities.empty()) {
        uint32_t id = freeEntities.back();
        freeEntities.pop_back();
        // masks は既に確保済み
        masks[id].reset(); // マスクをリセット
        // generation は destroy() 時にインクリメント済みなので、その値を使う
        return Entity{ id, generations[id] };
    }
    uint32_t id = nextID++;
    masks.emplace_back(); // 新しいID用のマスク領域を確保
    generations.push_back(0);
    return Entity{ id, 0 };
}

void EntityManager::destroy(Entity e)
{
    if(e.id >= generations.size()) return; // 安全策

    // 世代をインクリメントして古いハンドルを無効化
    ++generations[e.id];
    masks[e.id].reset(); // 破壊されたらコンポーネント情報を消す
    freeEntities.push_back(e.id);
}

void EntityManager::clear()
{
    nextID = 0;
    freeEntities.clear();
    masks.clear();
    generations.clear();
}

ComponentMask EntityManager::getMask(Entity e) const
{
    return masks[e.id];
}

void EntityManager::setMask(Entity e, ComponentMask mask)
{
    masks[e.id] = mask;
}

bool EntityManager::isAlive(Entity e) const
{
    // 世代管理により高速判定
    return e.id < static_cast<uint32_t>(generations.size()) && generations[e.id] == e.generation;
}

std::vector<Entity> EntityManager::getLivingEntities() const
{
    std::vector<Entity> living;

    // generations のサイズ（現在の最大ID数）をループ
    for(uint32_t i = 0; i < static_cast<uint32_t>(generations.size()); ++i) {
        Entity e{i, generations[i]};
        if(isAlive(e)) {
            living.push_back(e);
        }
    }
    return living;
}
}
