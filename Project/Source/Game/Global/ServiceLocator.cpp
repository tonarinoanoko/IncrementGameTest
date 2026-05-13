#include "ServiceLocator.h"

namespace MyGame {

// --- static メンバ変数の実体定義 ---
std::unordered_map<std::type_index, std::shared_ptr<void>> ServiceLocator::_all_services;
std::vector<std::shared_ptr<IGlobalService>> ServiceLocator::_updatable_services;

// --- static 関数の実装 ---
void ServiceLocator::updateAll()
{
    for(auto& service : _updatable_services) {
        if(service) {
            service->update();
        }
    }
}

void ServiceLocator::clear()
{
    _all_services.clear();
    _updatable_services.clear();
}
}