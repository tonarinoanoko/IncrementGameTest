#include "BP_Generator.h"
#include "Game/Component/Common/Timer/Co_Timer.h"
#include "Game/Component/Generator/Co_Generator.h"
#include "Game/Component/Common/Render/Co_Render2D.h"


namespace MyGame {
MyECS::Entity BP_Generator::Create(MyECS::World& world, MyECS::SystemManager& systems, int index)
{
    auto entity = world.createEntity();

    Co_Timer timer;
    timer.interval = 1.0f + (0.5f * index);
    timer.is_loop = true;
    world.addComponent<Co_Timer>(entity, timer);

    Co_Generator gene;
    gene.base_production = 1.0f * (index + 1.0f);
    gene.level = 1;
    world.addComponent<Co_Generator>(entity, gene);

    Co_Render2D render;
    render.base_pos.set(25.0f, 40.0f + (25 * index));

    {
        Render2D_Element element;
        element.type = ERender2D_Type::Text;
        element.transform.pos.set(2.0f, 2.0f);
        element.text = "generator";
        render.elements.push_back(element);
    }

    {
        Render2D_Element element;
        element.type = ERender2D_Type::Text;
        element.transform.pos.set(100.0f, 2.0f);
        element.setTextValue(gene.base_production, 0);
        render.elements.push_back(element);
    }

    {
        Render2D_Element element;
        element.type = ERender2D_Type::Box;
        element.size.set(150, 20);
        render.elements.push_back(element);
    }
    world.addComponent<Co_Render2D>(entity, render);

    return entity;
}

}