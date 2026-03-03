#include "TitleScene.h"
#include "GameScene.h"
#include "../core/Application.h"
#include "../gfx/Renderer.h"

TitleScene::TitleScene()
{

}

TitleScene::~TitleScene()
{

}

void TitleScene::OnEnter(Application& app)
{
    m_world.SetCameraPosition(0.f, 0.f);
    // 예시: 타이틀 이미지가 있다고 가정
    m_titleTex = app.Resource().LoadTexture("Resource/kirby.png");

    GameObject& obj = m_world.Spawn();
    m_title = &obj;

    m_title->transform.position = { 0.f, 0.f };
    m_title->transform.z = 0;

    auto& spr = m_title->AddComponent<SpriteRendererComponent>(m_titleTex);
    spr.SetSize(531.f, 700.f);
}

void TitleScene::OnExit(Application& app)
{
    m_world.Clear();
    m_title = nullptr;
    m_titleTex.reset();
}

void TitleScene::HandleEvent(Application & app, const SDL_Event & e) 
{
	// ESC 누르면 종료
	if (app.GetInput().WasPressed(SDLK_ESCAPE)) {
		app.quit();
	}
    if (app.GetInput().WasPressed(SDLK_RETURN)) {
        app.SetScene(std::make_unique<GameScene>());
    }
}

void TitleScene::Update(Application& app, float dt) 
{
    m_world.Update(dt);
}

void TitleScene::Render(Application& app, DrawQueue& q) 
{
    m_world.Render(q);

	//int w, h;
	////app에서 윈도우 
	//SDL_GetWindowSize(app.GetWindow(), &w, &h);
	//// 화면 중앙에 흰 사각형 하나(TitleScene이 떴다 확인용)
	//SDL_FRect r{ (w - 300.0f) * 0.5f, (h - 120.0f) * 0.5f, 300.0f, 120.0f };

	//q.AddRectFilled(r, { 255,255,255,255 }, 10); //드로우 큐에 렌더를 쌓아두기
}