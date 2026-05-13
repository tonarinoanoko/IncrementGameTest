#pragma once
#include "Game/Global/IGlobalService.h"
#include "InputAction.h"
#include <unordered_map>

namespace MyGame {
class InputManager : public IGlobalService {
public:
    InputManager();
    void update() override; // IGlobalServiceの実装

    bool isPressed(InputAction action) const;
    bool isTriggered(InputAction action) const;
    void getMousePos(int& x, int& y) const;
    bool isMouseTriggered(int buttonMask) const;

private:
    std::unordered_map<InputAction, int> _key_config;
    char _key_state[256];
    char _prev_key_state[256];
    int _mouse_x, _mouse_y, _mouse_buttons, _prev_mouse_buttons;
};
}