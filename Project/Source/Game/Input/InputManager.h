#pragma once
#include "Game/Global/IGlobalService.h"
#include "InputAction.h"
#include <unordered_map>

namespace MyGame {
class InputManager : public IGlobalService {
private:
    std::unordered_map<InputAction, int> keyConfig;
    char keyState[256];
    char prevKeyState[256];
    int mouseX, mouseY, mouseButtons, prevMouseButtons;

public:
    InputManager();
    void update() override; // IGlobalServiceの実装

    bool isPressed(InputAction action) const;
    bool isTriggered(InputAction action) const;
    void getMousePos(int& x, int& y) const;
    bool isMouseTriggered(int buttonMask) const;
};
}