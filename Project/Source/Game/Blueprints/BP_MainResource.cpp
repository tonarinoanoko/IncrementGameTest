#include "BP_MainResource.h"
#include "Game/Component/Resource/Co_MainResource.h"


namespace MyGame {

MyECS::Entity BP_MainResource::Create(MyECS::World& world, MyECS::SystemManager& systems)
{
    auto entity = world.createEntity();

    Co_MainResource res;
    world.addComponent<Co_MainResource>(entity, res);

    return entity;
}

}