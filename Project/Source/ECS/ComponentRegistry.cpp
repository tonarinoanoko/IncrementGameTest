#include "ComponentRegistry.h"

namespace MyECS {

ComponentRegistry::ComponentRegistry() : _next_id(0)
{
}

ComponentRegistry::~ComponentRegistry()
{
}

uint32_t ComponentRegistry::getIDInternal(const std::string& name)
{
    auto it = _type_to_id.find(name);
    if(it != _type_to_id.end()) {
        return it->second;
    }

    // 初めての型名なら新しいIDを発行
    uint32_t id = _next_id++;
    _type_to_id[name] = id;
    return id;
}

uint32_t ComponentRegistry::getIDByTypeName(const std::string& name)
{
    return getIDInternal(name);
}

}