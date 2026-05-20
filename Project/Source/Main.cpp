#include "DxLib.h"
#include "ECS/World.h"
#include "Game/Time/TimeManager.h"
#include "Game/Scene/SceneManager.h"
#include "Game/Global/ServiceLocator.h"
#include "Game/Input/InputManager.h"
#include "Game/Scene/SceneRequestManager.h"
#include "Game/MasterData/MasterDataManager.h"


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // DxLib初期化
    ChangeWindowMode(TRUE);
    if(DxLib_Init() == -1) {
        return -1;
    }

    SetDrawScreen(DX_SCREEN_BACK);

    // 起動時に登録
    MyGame::ServiceLocator::registerService<MyGame::TimeManager>(std::make_shared<MyGame::TimeManager>());
    MyGame::ServiceLocator::registerService<MyGame::InputManager>(std::make_shared<MyGame::InputManager>());
    MyGame::ServiceLocator::registerService<MyGame::SceneRequestManager>(std::make_shared<MyGame::SceneRequestManager>());
    MyGame::ServiceLocator::registerService<MyGame::MasterDataManager>(std::make_shared<MyGame::MasterDataManager>());

    MyECS::World world;

    MyGame::SceneManager scene_manager;
    scene_manager.changeScene(MyGame::SceneType::Start, world);

    // --- メインループ ---
    while(ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
        ClearDrawScreen();

        MyGame::ServiceLocator::updateAll();

        scene_manager.update(world);

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}