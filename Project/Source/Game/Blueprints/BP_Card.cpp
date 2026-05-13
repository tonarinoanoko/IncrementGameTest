#include "BP_Card.h"
#include "Game/Global/ServiceLocator.h"
#include "Game/MasterData/MasterDataManager.h"
//#include "Game/MasterData/Datas/MD_Card.h"
//#include "Game/Components/Common/Co_Card.h"
//#include "Game/Systems/Sys_ThunderStorm.h"

namespace MyGame {
MyECS::Entity BP_Card::Create(MyECS::World& world, MyECS::SystemManager& systems, const std::string& cardId)
{
    /*
    // 1. マスタ取得
    auto const masterMgr = ServiceLocator::get<MasterDataManager>();
    auto const cardMaster = masterMgr->get<MD_Card>();
    auto const data = cardMaster->get(cardId);

    if(!data) {
        return MyECS::Entity();
    }

    // 2. Entity生成
    auto entity = world.createEntity();

    // 3. 基本的なコンポーネント設定
    /*
    Co_Card base;
    base.name = data->name;
    base.cost = data->cost;
    world.addComponent<Co_Card>(entity, base);

    // 4. 【重要】特定のSystemが必要なら追加する
    // SystemManager 側で「既に存在すれば無視する」実装になっている前提
    if(data->requiredSystem == "Sys_ThunderStorm") {
        systems.addSystem<Sys_ThunderStorm>();
    }
    */

    auto entity = world.createEntity();
    return entity;
}
}