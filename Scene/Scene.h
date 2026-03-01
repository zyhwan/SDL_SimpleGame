#pragma once
#include <SDL3/SDL.h>

class Application;

//추상 클래스 (Scene)
class Scene {
public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual void OnEnter(Application& app) {}
    virtual void OnExit(Application& app) {}

    virtual void HandleEvent(Application& app, const SDL_Event& e) = 0;
    virtual void Update(Application& app, float dt) = 0;
    virtual void Render(Application& app, SDL_Renderer* renderer) = 0;
};