#pragma once

#include "EntityManager.h"
#include "ComponentRegistry.h"
#include "ComponentPool.h"
#include <unordered_map>
#include <string>
#include <memory>
#include <typeindex>
#include <vector>

namespace MyECS {
class World {
private:
    EntityManager entityManager;
    ComponentRegistry componentRegistry;

    // 型名をキーにして、各コンポーネントのプール（実データ）を保持
    std::unordered_map<std::string, std::shared_ptr<IComponentPool>> componentPools;

    // 型Tに対応するプールを内部的に取得・生成する
    template<typename T>
    std::shared_ptr<ComponentPool<T>> getComponentPool()
    {
        std::string typeName = typeid(T).name();
        if(componentPools.find(typeName) == componentPools.end()) {
            componentPools[typeName] = std::make_shared<ComponentPool<T>>();
        }
        return std::static_pointer_cast<ComponentPool<T>>(componentPools[typeName]);
    }

public:
    World() = default;

    // --- Entity 操作 ---
    Entity createEntity()
    {
        return entityManager.create();
    }

    void destroyEntity(Entity e)
    {
        // 全てのプールからこのEntityのデータを削除
        for(auto& [name, pool] : componentPools) {
            pool->remove(e);
        }
        entityManager.destroy(e);
    }

    // --- Component 操作 ---
    template <typename T>
    void addComponent(Entity e, const T& component)
    {
        auto pool = getComponentPool<T>();
        pool->add(e, component);

        // ビットマスクを更新
        uint32_t tid = componentRegistry.getID<T>();
        ComponentMask mask = entityManager.getMask(e);
        mask.set(tid);
        entityManager.setMask(e, mask);
    }

    template <typename T>
    T& getComponent(Entity e)
    {
        return getComponentPool<T>()->get(e);
    }

    template <typename T>
    bool hasComponent(Entity e)
    {
        uint32_t tid = componentRegistry.getID<T>();
        return entityManager.getMask(e).test(tid);
    }

    template<typename... Components>
    std::vector<Entity> view()
    {
        std::vector<Entity> results;

        // 1. EntityManager から生存中の全 Entity を取得
        auto livingEntities = entityManager.getLivingEntities();

        for(const auto& entity : livingEntities) {
            // 2. 各コンポーネントの Pool に対して「この Entity を持っているか」を確認
            // 既存の getComponentPool<C>() が返すオブジェクトの has(entity) を使用
            // (Pool側の関数名が contains の場合は適宜読み替えてください)
            bool hasAll = (getComponentPool<Components>()->has(entity) && ...);

            if(hasAll) {
                results.push_back(entity);
            }
        }
        return results;
    }

    // --- Query 機能 (可変長引数テンプレート) ---
    template <typename... Components>
    std::vector<Entity> query()
    {
        ComponentMask targetMask;
        // 全ての型のIDを取得して合成
        uint32_t ids[] = {componentRegistry.getID<Components>()...};
        for(uint32_t id : ids) targetMask.set(id);

        std::vector<Entity> results;
        uint32_t capacity = entityManager.getCapacity();

        for(uint32_t i = 0; i < capacity; ++i) {
            Entity e{i};
            // 生存確認 ＆ マスク一致確認
            if(entityManager.isAlive(e) && (entityManager.getMask(e) & targetMask) == targetMask) {
                results.push_back(e);
            }
        }
        return results;
    }

    // --- システム管理 (セーブ/ロード/リセット) ---
    void clear();
    void save(const std::string& filename);
    void load(const std::string& filename);
};
}