#pragma once
#include <vector>
#include <memory>
#include "ISystem.h"

namespace MyECS {
class World;

class SystemManager {
public:
    SystemManager();
    ~SystemManager();

    // テンプレート：これだけはヘッダーに置く必要がある
    template<typename T, typename... Args>
    void addSystem(Args&&... args)
    {
        _systems.push_back(std::make_shared<T>(std::forward<Args>(args)...));
        _needs_sort = true;
    }

    void updateAll(World& world);

private:
    std::vector<std::shared_ptr<ISystem>> _systems;
    bool _needs_sort;

    // 内部で使用するソート関数
    void sortSystems();
};
}