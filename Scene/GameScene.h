#pragma once
#include "Scene.h"

class GameScene : public Scene {
public:
    GameScene();
    ~GameScene();

    void OnEnter(Application& app) override;
    void OnExit(Application& app) override;

    void HandleEvent(Application& app, const SDL_Event& e) override;
    void Update(Application& app, float dt) override;
    void Render(Application& app, DrawQueue& q) override;

private:
    // 테스트용: 이동하는 사각형 위치/속도
    float m_x = 472.0f;
    float m_y = 340.0f;
    float m_speed = 250.0f; // px/s

    TextureHandle m_Tex;
};