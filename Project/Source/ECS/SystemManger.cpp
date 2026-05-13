#include "SystemManager.h"
#include <algorithm>

namespace MyECS {

SystemManager::SystemManager() : needsSort(false) {}
SystemManager::~SystemManager() {}

void SystemManager::sortSystems()
{
    if(!needsSort) return;

    std::sort(systems.begin(), systems.end(),
              [](const std::shared_ptr<ISystem>& a, const std::shared_ptr<ISystem>& b)
    {
        return a->getPriority() < b->getPriority();
    });

    needsSort = false;
}

void SystemManager::updateAll(World& world, float deltaTime)
{
    // 更新前に必要があればソート
    sortSystems();

    // 全システムの更新実行
    for(auto& system : systems) {
        system->update(world, deltaTime);
    }
}
}