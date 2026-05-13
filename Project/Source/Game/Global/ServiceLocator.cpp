#include "ServiceLocator.h"

namespace MyGame {

// --- static メンバ変数の実体定義 ---
std::unordered_map<std::type_index, std::shared_ptr<void>> ServiceLocator::allServices;
std::vector<std::shared_ptr<IGlobalService>> ServiceLocator::updatableServices;

// --- static 関数の実装 ---
void ServiceLocator::updateAll()
{
    for(auto& service : updatableServices) {
        if(service) {
            service->update();
        }
    }
}

void ServiceLocator::clear()
{
    allServices.clear();
    updatableServices.clear();
}
}