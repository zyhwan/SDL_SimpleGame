#pragma once
#include "Scene.h"

class TitleScene : public Scene {
public:
    TitleScene();
    ~TitleScene();

    //Application 본체를 참조하는 이유는 각 씬의 함수에서 application(엔진)에 기능을 요청할 경우 생긴다.

    void OnEnter(Application& app) override {}
    void OnExit(Application& app) override {}

    void HandleEvent(Application& app, const SDL_Event& e) override;
    void Update(Application& app, float dt) override;
    void Render(Application& app, DrawQueue& q) override; // DrawQueue에 그리기 명령만 적재
};