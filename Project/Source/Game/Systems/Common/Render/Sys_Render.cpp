#include "Sys_Render.h"
#include "Game/Component/Common/Co_Transform.h"
#include "Game/Component/Common/Co_Sprite.h"
#include "ECS/World.h"
#include "DxLib.h"
#include <vector>
#include <algorithm>

namespace MyGame {

// 描画情報を一時的に保持する構造体
struct RenderOrder {
    int priority;
    const Co_Transform* trans;
    const Co_Sprite* sprite;
};

void Sys_Render::update(MyECS::World& world, float deltaTime)
{
    // 1. Transform と Sprite を持つ Entity を抽出
    auto view = world.view<Co_Transform, Co_Sprite>();
    std::vector<RenderOrder> orders;

    for(auto entity : view) {
        const auto& trans = world.getComponent<Co_Transform>(entity);
        const auto& sprite = world.getComponent<Co_Sprite>(entity);

        // 非表示またはハンドルが無効ならスキップ
        if(!sprite.isVisible || sprite.handle == -1) continue;

        orders.push_back({sprite.priority, &trans, &sprite});
    }

    // 2. Priority（描画順）で昇順ソート（数値が小さいほど奥、大きいほど手前）
    std::sort(orders.begin(), orders.end(), [](const RenderOrder& a, const RenderOrder& b)
    {
        return a.priority < b.priority;
    });

    // 3. ソート順に従って DxLib で描画
    for(const auto& order : orders) {
        DrawRotaGraphF(
            order.trans->x,
            order.trans->y,
            (double)order.trans->scale,
            (double)order.trans->angle,
            order.sprite->handle,
            TRUE // 透過有効
        );
    }
}
}