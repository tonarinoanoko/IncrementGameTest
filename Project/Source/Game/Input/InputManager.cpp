#include "InputManager.h"
#include "DxLib.h"

namespace MyGame {

InputManager::InputManager()
    : _mouse_x(0), _mouse_y(0), _mouse_buttons(0), _prev_mouse_buttons(0)
{
    // キーコンフィグの初期設定
    _key_config[InputAction::Decision] = KEY_INPUT_SPACE;
    _key_config[InputAction::Cancel] = KEY_INPUT_ESCAPE;
    _key_config[InputAction::MoveUp] = KEY_INPUT_W;
    _key_config[InputAction::MoveDown] = KEY_INPUT_S;
    _key_config[InputAction::MoveLeft] = KEY_INPUT_A;
    _key_config[InputAction::MoveRight] = KEY_INPUT_D;
    _key_config[InputAction::Menu] = KEY_INPUT_TAB;

    // 状態配列の初期化
    for(int i = 0; i < 256; ++i) {
        _key_state[i] = 0;
        _prev_key_state[i] = 0;
    }
}

void InputManager::update()
{
    // 1. 前フレームの状態をコピー
    for(int i = 0; i < 256; ++i) {
        _prev_key_state[i] = _key_state[i];
    }
    _prev_mouse_buttons = _mouse_buttons;

    // 2. 最新の入力状態を取得 (DxLib関数)
    GetHitKeyStateAll(_key_state);
    GetMousePoint(&_mouse_x, &_mouse_y);
    _mouse_buttons = GetMouseInput();
}

// --- キーボード判定 ---

bool InputManager::isPressed(InputAction action) const
{
    auto it = _key_config.find(action);
    if(it == _key_config.end()) return false;

    int code = it->second;
    // 1であれば「現在押されている」
    return (_key_state[code] == 1);
}

bool InputManager::isTriggered(InputAction action) const
{
    auto it = _key_config.find(action);
    if(it == _key_config.end()) return false;

    int code = it->second;
    // 「今1」かつ「前は0」なら、押された瞬間
    return (_key_state[code] == 1 && _prev_key_state[code] == 0);
}

// --- マウス判定 ---

void InputManager::getMousePos(int& x, int& y) const
{
    x = _mouse_x;
    y = _mouse_y;
}

bool InputManager::isMouseTriggered(int buttonMask) const
{
    // 引数 buttonMask には MOUSE_INPUT_LEFT 等が入る
    // 「今ビットが立っている」かつ「前は立っていなかった」
    return ((_mouse_buttons & buttonMask) != 0 && (_prev_mouse_buttons & buttonMask) == 0);
}

}