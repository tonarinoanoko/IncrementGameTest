#include "InputManager.h"
#include "DxLib.h"

namespace MyGame {

InputManager::InputManager()
    : mouseX(0), mouseY(0), mouseButtons(0), prevMouseButtons(0)
{
    // キーコンフィグの初期設定
    keyConfig[InputAction::Decision] = KEY_INPUT_SPACE;
    keyConfig[InputAction::Cancel] = KEY_INPUT_ESCAPE;
    keyConfig[InputAction::MoveUp] = KEY_INPUT_W;
    keyConfig[InputAction::MoveDown] = KEY_INPUT_S;
    keyConfig[InputAction::MoveLeft] = KEY_INPUT_A;
    keyConfig[InputAction::MoveRight] = KEY_INPUT_D;
    keyConfig[InputAction::Menu] = KEY_INPUT_TAB;

    // 状態配列の初期化
    for(int i = 0; i < 256; ++i) {
        keyState[i] = 0;
        prevKeyState[i] = 0;
    }
}

void InputManager::update()
{
    // 1. 前フレームの状態をコピー
    for(int i = 0; i < 256; ++i) {
        prevKeyState[i] = keyState[i];
    }
    prevMouseButtons = mouseButtons;

    // 2. 最新の入力状態を取得 (DxLib関数)
    GetHitKeyStateAll(keyState);
    GetMousePoint(&mouseX, &mouseY);
    mouseButtons = GetMouseInput();
}

// --- キーボード判定 ---

bool InputManager::isPressed(InputAction action) const
{
    auto it = keyConfig.find(action);
    if(it == keyConfig.end()) return false;

    int code = it->second;
    // 1であれば「現在押されている」
    return (keyState[code] == 1);
}

bool InputManager::isTriggered(InputAction action) const
{
    auto it = keyConfig.find(action);
    if(it == keyConfig.end()) return false;

    int code = it->second;
    // 「今1」かつ「前は0」なら、押された瞬間
    return (keyState[code] == 1 && prevKeyState[code] == 0);
}

// --- マウス判定 ---

void InputManager::getMousePos(int& x, int& y) const
{
    x = mouseX;
    y = mouseY;
}

bool InputManager::isMouseTriggered(int buttonMask) const
{
    // 引数 buttonMask には MOUSE_INPUT_LEFT 等が入る
    // 「今ビットが立っている」かつ「前は立っていなかった」
    return ((mouseButtons & buttonMask) != 0 && (prevMouseButtons & buttonMask) == 0);
}

}