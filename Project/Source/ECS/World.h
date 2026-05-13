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
public:
    World() = default;

    // --- Entity 操作 ---
    Entity createEntity()
    {
        return _entity_manager.create();
    }

    void destroyEntity(Entity e)
    {
        // 全てのプールからこのEntityのデータを削除
        for(auto& [name, pool] : _component_pools) {
            pool->remove(e);
        }
        _entity_manager.destroy(e);
    }

    // --- Component 操作 ---
    template <typename T>
    void addComponent(Entity e, const T& component)
    {
        auto pool = getComponentPool<T>();
        pool->add(e, component);

        // ビットマスクを更新
        uint32_t tid = _component_registry.getID<T>();
        ComponentMask mask = _entity_manager.getMask(e);
        mask.set(tid);
        _entity_manager.setMask(e, mask);
    }

    template <typename T>
    T& getComponent(Entity e)
    {
        return getComponentPool<T>()->get(e);
    }

    template <typename T>
    bool hasComponent(Entity e)
    {
        uint32_t tid = _component_registry.getID<T>();
        return _entity_manager.getMask(e).test(tid);
    }

    template<typename... Components>
    std::vector<Entity> view()
    {
        std::vector<Entity> results;

        // 1. EntityManager から生存中の全 Entity を取得
        auto living_entities = _entity_manager.getLivingEntities();

        for(const auto& entity : living_entities) {
            // 2. 各コンポーネントの Pool に対して「この Entity を持っているか」を確認
            // 既存の getComponentPool<C>() が返すオブジェクトの has(entity) を使用
            // (Pool側の関数名が contains の場合は適宜読み替えてください)
            bool has_all = (getComponentPool<Components>()->has(entity) && ...);

            if(has_all) {
                results.push_back(entity);
            }
        }
        return results;
    }

    // --- Query 機能 (可変長引数テンプレート) ---
    template <typename... Components>
    std::vector<Entity> query()
    {
        ComponentMask target_mask;
        // 全ての型のIDを取得して合成
        uint32_t ids[] = {_component_registry.getID<Components>()...};
        for(uint32_t id : ids) {
            target_mask.set(id);
        }

        std::vector<Entity> results;
        uint32_t capacity = _entity_manager.getCapacity();

        for(uint32_t i = 0; i < capacity; ++i) {
            Entity e{i};
            // 生存確認 ＆ マスク一致確認
            if(_entity_manager.isAlive(e) && (_entity_manager.getMask(e) & target_mask) == target_mask) {
                results.push_back(e);
            }
        }
        return results;
    }

    // --- システム管理 (セーブ/ロード/リセット) ---
    void clear();
    void save(const std::string& filename);
    void load(const std::string& filename);

private:
    EntityManager _entity_manager;
    ComponentRegistry _component_registry;

    // 型名をキーにして、各コンポーネントのプール（実データ）を保持
    std::unordered_map<std::string, std::shared_ptr<IComponentPool>> _component_pools;

    // 型Tに対応するプールを内部的に取得・生成する
    template<typename T>
    std::shared_ptr<ComponentPool<T>> getComponentPool()
    {
        std::string type_name = typeid(T).name();
        if(_component_pools.find(type_name) == _component_pools.end()) {
            _component_pools[type_name] = std::make_shared<ComponentPool<T>>();
        }
        return std::static_pointer_cast<ComponentPool<T>>(_component_pools[type_name]);
    }
};
}