#include "TitleScene.h"
#include "../core/Application.h"
#include "GameScene.h"

TitleScene::TitleScene()
{

}

TitleScene::~TitleScene()
{

}

void TitleScene::HandleEvent(Application & app, const SDL_Event & e) {
	// ESC 누르면 종료
	if (app.GetInput().WasPressed(SDLK_ESCAPE)) {
		app.quit();
	}
}

void TitleScene::Update(Application& app, float dt) {
    if (app.GetInput().WasPressed(SDLK_ESCAPE)) {
        app.quit();
        return;
    }

    // Enter 누르면 GameScene으로
    if (app.GetInput().WasPressed(SDLK_RETURN)) {
        app.SetScene(std::make_unique<GameScene>());
        return;
    }
}

void TitleScene::Render(Application& app, SDL_Renderer* renderer) {
    int w, h;
    //app에서 윈도우 
    SDL_GetWindowSize(app.GetWindow(), &w, &h);
    // 화면 중앙에 흰 사각형 하나(TitleScene이 떴다 확인용)
    SDL_FRect r{ (w - 300.0f) * 0.5f, (h - 120.0f) * 0.5f, 300.0f, 120.0f };

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderFillRect(renderer, &r);
}