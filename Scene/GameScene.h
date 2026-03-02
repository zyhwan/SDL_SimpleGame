#pragma once
#include "Scene.h"
#include "../World/World.h" // 경로는 네 프로젝트 구조에 맞게 조정
#include "../World/SpriteRendererComponent.h"

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
    World m_world;
    GameObject* m_player = nullptr;  // 월드가 소유, 씬은 포인터만 보관

    float m_speed = 500.0f; // px/s

    TextureHandle m_Tex;
};