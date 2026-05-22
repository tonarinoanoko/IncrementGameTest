#include "BP_MainResource.h"
#include "Game/Component/Resource/Co_MainResource.h"
#include "Game/Component/Common/Render/Co_Render2D.h"


namespace MyGame {

MyECS::Entity BP_MainResource::Create(MyECS::World& world, MyECS::SystemManager& systems)
{
    auto entity = world.createEntity();

    Co_MainResource res;
    res.amout_text_index = 1;
    world.addComponent<Co_MainResource>(entity, res);

    Co_Render2D render;
    render.base_pos.set(10.0f, 10.0f);

    {
        Render2D_Element element;
        element.type = ERender2D_Type::Text;
        element.transform.pos.set(2.0f, 2.0f);
        element.text = "resource";
        render.elements.push_back(element);
    }

    {
        Render2D_Element element;
        element.type = ERender2D_Type::Text;
        element.transform.pos.set(100.0f, 2.0f);
        element.text = "value";
        render.elements.push_back(element);
    }

    {
        Render2D_Element element;
        element.type = ERender2D_Type::Box;
        element.size.set(200, 20);
        render.elements.push_back(element);
    }

    world.addComponent<Co_Render2D>(entity, render);

    return entity;
}

}