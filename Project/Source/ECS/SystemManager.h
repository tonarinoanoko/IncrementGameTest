#pragma once
#include <vector>
#include <memory>
#include "ISystem.h"

namespace MyECS {
class World;

class SystemManager {
private:
    std::vector<std::shared_ptr<ISystem>> systems;
    bool needsSort;

    // 内部で使用するソート関数
    void sortSystems();

public:
    SystemManager();
    ~SystemManager();

    // テンプレート：これだけはヘッダーに置く必要がある
    template<typename T, typename... Args>
    void addSystem(Args&&... args)
    {
        systems.push_back(std::make_shared<T>(std::forward<Args>(args)...));
        needsSort = true;
    }

    void updateAll(World& world, float deltaTime);
};
}