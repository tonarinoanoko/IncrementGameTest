#pragma once
#include "Entity.h"
#include <memory>
#include <unordered_map>
#include <nlohmann/json.hpp>

namespace MyECS {
// 型を問わずプールを操作するための基底クラス
class IComponentPool {
public:
    virtual ~IComponentPool() = default;

    // Entityが削除されたときに、そのEntityに紐づくデータを消す用
    virtual void remove(Entity e) = 0;
    virtual void clear() = 0;

    // AIのシミュレーション用に自分自身のディープコピーを作成する
    virtual std::unique_ptr<IComponentPool> clone() const = 0;

    virtual std::vector<Entity> getAliveEntities() = 0;

    virtual nlohmann::json serialize() = 0;
    virtual void deserialize(const nlohmann::json& j) = 0;
};

template <typename T>
class ComponentPool : public IComponentPool {
private:
    std::unordered_map<Entity, T> data;

public:
    ComponentPool() = default;

    // コピーコンストラクタ（cloneで使用）
    ComponentPool(const ComponentPool& other) : data(other.data) {}

    void set(Entity e, const T& component)
    {
        data[e] = component;
    }

    T& get(Entity e)
    {
        return data.at(e);
    }

    bool has(Entity e) const
    {
        return data.find(e) != data.end();
    }

    void remove(Entity e) override
    {
        data.erase(e);
    }

    void clear() override
    {
        data.clear();
    }

    std::unique_ptr<IComponentPool> clone() const override
    {
        return std::make_unique<ComponentPool<T>>(*this);
    }

    std::vector<Entity> getAliveEntities() override
    {
        std::vector<Entity> result;
        for(auto const& [entity, data] : data) {
            result.push_back(entity);
        }
        return result;
    }

    nlohmann::json serialize() override
    {
        nlohmann::json arr = nlohmann::json::array();
        for(auto& [entity, data] : data) {
            nlohmann::json entry;
            entry["eid"] = entity.id;
            entry["data"] = data; // T型がnlohmann::jsonに対応している必要があります
            arr.push_back(entry);
        }
        return arr;
    }

    void deserialize(const nlohmann::json& j) override
    {
        data.clear();
        for(const auto& entry : j) {
            Entity e{entry["eid"].get<uint32_t>()};
            data[e] = entry["data"].get<T>();
        }
    }
};
}
