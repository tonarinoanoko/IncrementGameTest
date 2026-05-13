#pragma once
#include <memory>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include "IGlobalService.h"

namespace MyGame {
class ServiceLocator {
private:
    static std::unordered_map<std::type_index, std::shared_ptr<void>> allServices;
    static std::vector<std::shared_ptr<IGlobalService>> updatableServices;

public:
    template<typename T>
    static void registerService(std::shared_ptr<T> service)
    {
        allServices[std::type_index(typeid(T))] = service;
        auto updatable = std::dynamic_pointer_cast<IGlobalService>(service);
        if(updatable) updatableServices.push_back(updatable);
    }

    template<typename T>
    static std::shared_ptr<T> get()
    {
        auto it = allServices.find(std::type_index(typeid(T)));
        if(it != allServices.end()) return std::static_pointer_cast<T>(it->second);
        return nullptr;
    }

    static void updateAll();
    static void clear();
};
}