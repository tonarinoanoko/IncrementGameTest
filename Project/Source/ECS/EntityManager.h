#pragma once
#include "Entity.h"
#include <vector>

namespace MyECS {
class EntityManager {
public:
    EntityManager();
    Entity create();
    void destroy(Entity e);
    void clear();

    // クエリ用に追加
    ComponentMask getMask(Entity e) const;
    void setMask(Entity e, ComponentMask mask);
    uint32_t getCapacity() const { return static_cast<uint32_t>(_generations.size()); }

    // Entityが現在使われているか（ビット判定用）
    bool isAlive(Entity e) const;

    // 現在生存しているEntityの取得
    std::vector<Entity> getLivingEntities() const;

private:
    uint32_t _next_id = 0;
    // free_entities stores free ids
    std::vector<uint32_t> _free_entities;
    std::vector<ComponentMask> _masks; // 各Entityがどのコンポーネントを持つか
    std::vector<uint32_t> _generations; // 各IDの世代管理
};
}