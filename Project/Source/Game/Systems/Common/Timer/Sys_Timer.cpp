#include "Sys_Timer.h"
#include "ECS/World.h"
#include "Game/Global/ServiceLocator.h"
#include "Game/Component/Common/Timer/Co_Timer.h"


namespace MyGame {

void Sys_Timer::update(MyECS::World& world)
{
    auto delta_time = ServiceLocator::get<TimeManager>()->getDeltaTime();

    auto entities = world.query<Co_Timer>();
    for(auto& entity : entities) {
        auto& timer = world.getComponent<Co_Timer>(entity);
        if(timer.is_ready) {
            continue;
        }

        timer.elapsed += delta_time;
        if(timer.interval >= timer.elapsed) {
            timer.is_ready = true;
            timer.elapsed -= timer.interval;
        }
    }
}

}