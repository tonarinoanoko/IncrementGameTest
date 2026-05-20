#pragma once
#include <memory>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include "IGlobalService.h"
#include "Services.h"


namespace MyGame {
class ServiceLocator {
public:
    template<typename T>
    static void registerService(std::shared_ptr<T> service)
    {
        _all_services[std::type_index(typeid(T))] = service;
        auto updatable = std::dynamic_pointer_cast<IGlobalService>(service);
        if(updatable) _updatable_services.push_back(updatable);
    }

    template<typename T>
    static std::shared_ptr<T> get()
    {
        auto it = _all_services.find(std::type_index(typeid(T)));
        if(it != _all_services.end()) return std::static_pointer_cast<T>(it->second);
        return nullptr;
    }

    static void updateAll();
    static void clear();

private:
    static std::unordered_map<std::type_index, std::shared_ptr<void>> _all_services;
    static std::vector<std::shared_ptr<IGlobalService>> _updatable_services;
};
}