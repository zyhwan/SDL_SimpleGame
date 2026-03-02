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
    SDL_FPoint win = app.GetWindowSizeF();

    m_Tex = app.Resource().LoadTexture("Resource/bmp_Sample.bmp");

    // 오브젝트 생성 (예약 world의 update에서 오브젝트 등록함. 씬이 켜질때 생성할 오브젝트들을 미리 등록하는 것.)
    GameObject& obj = m_world.Spawn();
    m_player = &obj;

    // Transform 초기값
    m_player->transform.position = { win.x * 0.5f, win.y * 0.5f }; //화면의 중앙 값
    m_player->transform.z = 1;

    // 스프라이트 렌더러 컴포넌트 부착
    auto& sprite = m_player->AddComponent<SpriteRendererComponent>(m_Tex);

    // 스프라이트 크기 지정(없으면 기본값)
    sprite.SetSize(64.f, 64.f);

    // (선택) 원점/회전 쓰면
    //sprite.SetOrigin(32.f, 32.f);
}

void GameScene::OnExit(Application& app)
{
    m_world.Clear();
    m_player = nullptr;
    m_Tex.reset();
}

void GameScene::HandleEvent(Application& app, const SDL_Event& e)
{
    // 여기서는 "즉발 이벤트"만 처리 예시
    // 이동은 Update에서 키 상태로 처리하는 편이 일반적
    if (app.GetInput().WasPressed(SDLK_RETURN))
    {
        app.SetScene(std::make_unique<TitleScene>()); //타이틀 씬으로 전환
    }
    // ESC: 종료
    if (app.GetInput().WasPressed(SDLK_ESCAPE)) {
        app.quit();
        return;
    }
}

void GameScene::Update(Application& app, float dt)
{

    // WASD 이동 (누르고 있는 동안)
    if (m_player)
    {
        auto& p = m_player->transform.position;

        if (app.GetInput().IsDown(SDLK_A) || app.GetInput().IsDown(SDLK_LEFT)) p.x -= m_speed * dt;
        if (app.GetInput().IsDown(SDLK_D) || app.GetInput().IsDown(SDLK_RIGHT)) p.x += m_speed * dt;
        if (app.GetInput().IsDown(SDLK_S) || app.GetInput().IsDown(SDLK_DOWN)) p.y += m_speed * dt;
        if (app.GetInput().IsDown(SDLK_W) || app.GetInput().IsDown(SDLK_UP)) p.y -= m_speed * dt;
    }

    //스페이스를 "이번 프레임에 눌렀을 때만" 처리
    if (app.GetInput().WasPressed(SDLK_SPACE)) {
        // 점프/발사/상호작용 등 1회 트리거
    }

    if (m_player) //카메라가 플레이어를 따라가는 로직.
    {
        SDL_FPoint win = app.GetWindowSizeF();
        SDL_FPoint p = m_player->transform.position;

        m_world.SetCameraPosition(
            p.x - win.x * 0.5f,
            p.y - win.y * 0.5f
        );
    }

    //즉, World.Update는 단순 로직 업데이트가 아니라 오브젝트 등록 / 삭제 커밋까지 포함하는 "월드 틱"임.
    m_world.Update(dt);

}

void GameScene::Render(Application& app, DrawQueue& q)
{
    // 월드는 DrawQueue에 커맨드만 적재
    m_world.Render(q);

    // 배경(월드 좌표): 예를 들어 (0,0)~(2000,1200)짜리 월드
    SDL_FRect worldBg = { 0.f, 0.f, 2000.f, 1200.f };

    // 카메라 적용해서 "스크린 좌표"로 변환 후 DrawQueue에 추가
    SDL_FRect screenBg = m_world.Camera().WorldToScreenRect(worldBg);
    q.AddRectFilled(screenBg, { 255,0,0,255 }, -1000); // z 낮게(뒤에 깔리게)


}
