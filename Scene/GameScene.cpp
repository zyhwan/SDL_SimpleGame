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

    //----사용자 지정 키보드 설정----
    // 이동 축
    //m_bind.BindAxis("MoveX", { SDLK_A, SDLK_LEFT }, { SDLK_D, SDLK_RIGHT });
    //m_bind.BindAxis("MoveY", { SDLK_W, SDLK_UP }, { SDLK_S, SDLK_DOWN });

    //// 액션
    //m_bind.BindAction("Jump", { SDLK_SPACE, SDLK_W, SDLK_UP });
    //m_bind.BindAction("Fire", { SDLK_J, SDLK_Z });
    //m_bind.BindAction("Grenade", { SDLK_K, SDLK_X });
    //m_bind.BindAction("Pause", { SDLK_ESCAPE });

    //// 슬로모/테스트
    //m_bind.BindAction("SlowMo", { SDLK_F1 });
    //m_bind.BindAction("Pause0", { SDLK_F2 });
    //m_bind.BindAction("Resume", { SDLK_F3 });

    //---- 씬 내부 오브젝트 초기화----
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

        // 카메라 follow (dt=0이면 위치 변화 없음)
        SDL_FPoint win = app.GetWindowSizeF();
        SDL_FPoint pp = m_player->transform.position;
        m_world.SetCameraPosition(pp.x - win.x * 0.5f, pp.y - win.y * 0.5f);
    }

    //스페이스를 "이번 프레임에 눌렀을 때만" 처리
    if (app.GetInput().WasPressed(SDLK_SPACE)) {
        // 점프/발사/상호작용 등 1회 트리거
    }

    //즉, World.Update는 단순 로직 업데이트가 아니라 오브젝트 등록 / 삭제 커밋까지 포함하는 "월드 틱"임.
    m_world.Update(dt);

}


void GameScene::UpdateUI(Application& app, float unscaledDt)
{
    // Pause 토글(입력은 unscaled로 처리해도 무방)
    if (app.GetInput().WasPressed(SDLK_F2)) {
        app.GetTime().SetTimeScale(0.0f); // pause
    }
    if (app.GetInput().WasPressed(SDLK_F3)) {
        app.GetTime().SetTimeScale(1.0f); // resume
    }
    if (app.GetInput().WasPressed(SDLK_F1)) {
        app.GetTime().SetTimeScale(0.2f); // slow-mo 테스트
    }

    // UI 깜빡임(udt 기반): timeScale=0이어도 계속 변화해야 정상
    m_uiBlink += unscaledDt;
    if (m_uiBlink >= 0.5f) {          // 0.5초마다 토글
        m_uiBlink = 0.f;
        m_showPauseText = !m_showPauseText;
    }
}


void GameScene::Render(Application& app, DrawQueue& q)
{
    // 월드는 DrawQueue에 커맨드만 적재
    m_world.Render(q);

    // 화면 고정 UI(스크린 좌표): 카메라 적용하면 안 됨
    // 간단히 화면 상단에 UI 바처럼 그린다
    q.AddRectFilled({ 0.f, 0.f, 300.f, 40.f }, { 20,20,20,200 }, 100000);

    // timeScale == 0일 때만 'PAUSE' 표시(깜빡임 확인)
    if (app.GetTime().GetTimeScale() == 0.0f && m_showPauseText) {
        // "PAUSED" 대신 표시용 큰 사각형
        q.AddRectFilled({ 20.f, 60.f, 240.f, 80.f }, { 255,255,255,255 }, 100000);
        q.AddRectOutline({ 20.f, 60.f, 240.f, 80.f }, { 0,0,0,255 }, 100001);
    }


    // 배경(월드 좌표): 예를 들어 (0,0)~(2000,1200)짜리 월드
    SDL_FRect worldBg = { 0.f, 0.f, 2000.f, 1200.f };

    // 카메라 적용해서 "스크린 좌표"로 변환 후 DrawQueue에 추가
    SDL_FRect screenBg = m_world.Camera().WorldToScreenRect(worldBg);
    q.AddRectFilled(screenBg, { 255,0,0,255 }, -1000); // z 낮게(뒤에 깔리게)


}