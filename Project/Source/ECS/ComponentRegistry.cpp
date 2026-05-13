#include "ComponentRegistry.h"

namespace MyECS {

ComponentRegistry::ComponentRegistry() : nextID(0)
{
}

ComponentRegistry::~ComponentRegistry()
{
}

uint32_t ComponentRegistry::getIDInternal(const std::string& name)
{
    auto it = typeToID.find(name);
    if(it != typeToID.end()) {
        return it->second;
    }

    // 初めての型名なら新しいIDを発行
    uint32_t id = nextID++;
    typeToID[name] = id;
    return id;
}

uint32_t ComponentRegistry::getIDByTypeName(const std::string& name)
{
    return getIDInternal(name);
}

}