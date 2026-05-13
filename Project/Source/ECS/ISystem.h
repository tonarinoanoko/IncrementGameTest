#pragma once

namespace MyECS {
class World;

enum ESystemPriority {
    Input = 0,
    Logic = 500,
    Physics = 700,
    Render = 1000
};

class ISystem {
public:
    virtual ~ISystem() = default;

    // 毎フレーム実行される更新処理
    // AIシミュレーション時にも同じロジックを通すため、Worldを引数で受け取る
    virtual void update(World& world, float deltaTime) = 0;

    virtual int getPriority() const { return ESystemPriority::Logic; }
};
}