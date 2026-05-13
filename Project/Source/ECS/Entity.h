#pragma once
#include <cstdint>
#include <functional>
#include <bitset>

namespace MyECS {
const size_t MAX_COMPONENTS = 64;
using ComponentMask = std::bitset<MAX_COMPONENTS>;

struct Entity {
    static constexpr uint32_t INVALID_ID = 0xFFFFFFFF;

    uint32_t _id = INVALID_ID;
    uint32_t _generation = 0;

    bool operator==(const Entity& other) const
    {
        return _id == other._id && _generation == other._generation;
    }

    bool operator!=(const Entity& other) const
    {
        return !(*this == other);
    }

    // map のキーや、std::sort で使用するために必要
    bool operator< (const Entity& other) const
    {
        if(_id != other._id) return _id < other._id;
        return _generation < other._generation;
    }

    explicit operator bool() const
    {
        return _id != INVALID_ID; // NullEntity の id 条件
    }

    bool operator!() const
    {
        return _id == INVALID_ID;
    }

    bool isValid() const
    {
        return _id != INVALID_ID;
    }
};

inline constexpr Entity NullEntity{Entity::INVALID_ID, 0};
}

// std名前空間を拡張して、Entityのハッシュ計算方法を教える
namespace std {
template <>
struct hash<MyECS::Entity> {
    size_t operator()(const MyECS::Entity& e) const
    {
        // id と generation を組み合わせてハッシュを作る
        // 簡易な組み合わせで十分（必要ならより良い combine を使う）
        size_t h1 = hash<uint32_t>{}(e._id);
        size_t h2 = hash<uint32_t>{}(e._generation);
        return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
    }
};
}