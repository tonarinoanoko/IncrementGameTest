#include "SystemManager.h"
#include <algorithm>

namespace MyECS {

SystemManager::SystemManager() : _needs_sort(false) {}
SystemManager::~SystemManager() {}

void SystemManager::sortSystems()
{
    if(!_needs_sort) return;

    std::sort(_systems.begin(), _systems.end(),
              [](const std::shared_ptr<ISystem>& a, const std::shared_ptr<ISystem>& b)
    {
        return a->getPriority() < b->getPriority();
    });

    _needs_sort = false;
}

void SystemManager::updateAll(World& world)
{
    // 更新前に必要があればソート
    sortSystems();

    // 全システムの更新実行
    for(auto& system : _systems) {
        system->update(world);
    }
}
}