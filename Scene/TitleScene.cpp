#include "TitleScene.h"
#include "../core/Application.h"

void TitleScene::HandleEvent(Application& app, const SDL_Event& e) {
    // ESC 누르면 종료
    if (e.type == SDL_EVENT_KEY_DOWN) {
        if (e.key.keysym.sym == SDLK_ESCAPE) {
            app.Quit();
        }
    }
}

void TitleScene::Update(Application& app, float dt) {
    (void)app; (void)dt;
}

void TitleScene::Render(Application& app, SDL_Renderer* renderer) {
    (void)app;

    // 화면 중앙에 흰 사각형 하나(“TitleScene이 떴다” 확인용)
    SDL_FRect r;
    r.w = 300.0f;
    r.h = 120.0f;
    r.x = (1024.0f - r.w) * 0.5f;
    r.y = (768.0f - r.h) * 0.5f;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &r);
}