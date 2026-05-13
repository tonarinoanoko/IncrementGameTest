#pragma once

namespace MyGame {
    class IGlobalService {
    public:
        virtual ~IGlobalService() = default;
        virtual void update() = 0; // ServiceLocatorから自動で呼ばれる
    };
}