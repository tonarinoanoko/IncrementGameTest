#pragma once
#include "Entity.h"
#include <vector>

namespace MyECS {
class EntityManager {
private:
    uint32_t nextID = 0;
    // freeEntities stores free ids
    std::vector<uint32_t> freeEntities;
    std::vector<ComponentMask> masks; // 各Entityがどのコンポーネントを持つか
    std::vector<uint32_t> generations; // 各IDの世代管理

public:
    EntityManager();
    Entity create();
    void destroy(Entity e);
    void clear();

    // クエリ用に追加
    ComponentMask getMask(Entity e) const;
    void setMask(Entity e, ComponentMask mask);
    uint32_t getCapacity() const { return static_cast<uint32_t>(generations.size()); }

    // Entityが現在使われているか（ビット判定用）
    bool isAlive(Entity e) const;

    // 現在生存しているEntityの取得
    std::vector<Entity> getLivingEntities() const;
};
}