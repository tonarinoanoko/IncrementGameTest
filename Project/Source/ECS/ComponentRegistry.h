#pragma once
#include <unordered_map>
#include <string>
#include <typeinfo>
#include <cstdint>

namespace MyECS {
class ComponentRegistry {
public:
    ComponentRegistry();
    ~ComponentRegistry();

    // テンプレート関数はヘッダーに書く必要があります（実体化のため）
    template<typename T>
    uint32_t getID()
    {
        return getIDInternal(typeid(T).name());
    }

    // 文字列による取得も外部に公開
    uint32_t getIDByTypeName(const std::string& name);

private:
    std::unordered_map<std::string, uint32_t> _type_to_id;
    uint32_t _next_id = 0;

    // 内部実装はcppへ隠蔽
    uint32_t getIDInternal(const std::string& name);
};
}