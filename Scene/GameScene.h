#pragma once
#include "Scene.h"

class GameScene : public Scene {
public:
    GameScene();
    ~GameScene();

    void HandleEvent(Application& app, const SDL_Event& e) override;
    void Update(Application& app, float dt) override;
    void Render(Application& app, SDL_Renderer* renderer) override;

private:
    // 테스트용: 이동하는 사각형 위치/속도
    float m_x = 100.0f;
    float m_y = 100.0f;
    float m_speed = 250.0f; // px/s
};