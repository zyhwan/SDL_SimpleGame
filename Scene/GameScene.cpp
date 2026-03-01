#include "GameScene.h"
#include "../Core/Application.h"
#include "../gfx/Renderer.h"
#include "TitleScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::OnEnter(Application& app)
{
    m_Tex = app.Resource().LoadTexture("Resource/bmp_Sample.bmp");
}

void GameScene::OnExit(Application& app)
{
    m_Tex.reset();
}

void GameScene::HandleEvent(Application& app, const SDL_Event& e)
{
	(void)app; (void)e;
	// 이벤트는 Input이 처리하므로 여기선 비워도 됨

}

void GameScene::Update(Application& app, float dt)
{
    // ESC: 종료
    if (app.GetInput().WasPressed(SDLK_ESCAPE)) {
        app.quit();
        return;
    }

    if (app.GetInput().WasPressed(SDLK_RETURN)) {
        app.SetScene(std::make_unique<TitleScene>());
        return;
    }

    // WASD 이동 (누르고 있는 동안)
    float dx = 0.0f, dy = 0.0f;
    if (app.GetInput().IsDown(SDLK_A)) dx -= 1.0f;
    if (app.GetInput().IsDown(SDLK_D)) dx += 1.0f;
    if (app.GetInput().IsDown(SDLK_W)) dy -= 1.0f;
    if (app.GetInput().IsDown(SDLK_S)) dy += 1.0f;

    // 대각선 속도 증가 방지(정규화) - 간단 버전
    if (dx != 0.0f && dy != 0.0f) {
        dx *= 0.7071067f; // 1/sqrt(2)
        dy *= 0.7071067f;
    }

	m_x += dx * m_speed * dt;
	m_y += dy * m_speed * dt;
}

void GameScene::Render(Application& app, DrawQueue& q)
{
	SDL_FRect r{ m_x, m_y, 80.0f, 80.0f };

    //드로우 큐에 렌더를 쌓아두기
    q.AddRectFilled(r, { 0,200,255,255 }, 5); //기본 사각형 생성

    SDL_FRect dst{ 300, 0, 531.6, 700 };
    q.AddTexture(m_Tex, dst, 4); //샘플 텍스쳐 생성
}
