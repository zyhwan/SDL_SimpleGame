#pragma once
#include "Scene.h"

class TitleScene : Scene {
public:
    virtual void OnEnter(Application& app) {}
    virtual void OnExit() {}

    void HandleEvent(Application& app, const SDL_Event& e) override;
    void Update(Application& app, float dt) override;
    void Render(Application& app, SDL_Renderer* renderer) override;
};