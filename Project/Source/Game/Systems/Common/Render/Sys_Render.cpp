#include "Sys_Render.h"
#include "DxLib.h"
#include <vector>
#include <algorithm>

#include "ECS/World.h"
#include "Game/Component/Common/Render/Co_Render2D.h"

namespace MyGame {

// 描画情報を一時的に保持する構造体
struct RenderOrder {
    MyECS::Entity entity;
    int priority;
};

void Sys_Render::update(MyECS::World& world)
{
    // 1. Transform と Sprite を持つ Entity を抽出
    auto view = world.view<Co_Render2D>();
    std::vector<RenderOrder> orders;

    for(auto const& entity : view) {
        auto const& render = world.getComponent<Co_Render2D>(entity);

        if(render.is_visble == false) {
            continue;
        }

        orders.push_back({entity, render.priorty});
    }

    // 2. Priority（描画順）で昇順ソート（数値が小さいほど奥、大きいほど手前）
    std::sort(orders.begin(), orders.end(), [](RenderOrder const& a, RenderOrder const& b)
    {
        return a.priority < b.priority;
    });

    // 3. ソート順に従って DxLib で描画
    for(auto const& order : orders) {
        auto const& render = world.getComponent<Co_Render2D>(order.entity);

        for(auto element : render.elements) {

            Co_Vector2 pos;
            pos.x = render.base_pos.x + element.transform.pos.x;
            pos.y = render.base_pos.y + element.transform.pos.y;

            switch(element.type) {
            case ERender2D_Type::Sprite:
                if(element.handle == -1) {
                    continue;
                }

                DrawRotaGraphF(
                    pos.x, pos.y,
                    element.transform.scale,
                    element.transform.angle,
                    element.handle, TRUE);
                break;

            case ERender2D_Type::Box:
                DrawBox(pos.getIntX(), pos.getIntY(),
                        pos.getIntX() + element.size.getIntX(),
                        pos.getIntY() + element.size.getIntY(),
                        element.color.getColor(), FALSE);
                break;

            case ERender2D_Type::Text:
                DrawString(pos.getIntX(), pos.getIntY(), element.text.c_str(), element.color.getColor());
                break;
            }
        }
    }
}
}