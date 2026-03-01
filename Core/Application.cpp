#include "Application.h"
#include "../Scene/Scene.h"
// MyFirstSDLGame/
// core / Application.cpp
// scene / Scene.h
// 경로가 이런 경우일 경우 앞에 ../을 작성해줘야함.

Application::Application() = default;


Application::~Application()
{
	Shutdown();
}

bool Application::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) == false)
	{
		SDL_Log("SDL_Init failed: %s", SDL_GetError());
		return false;
	}
	// 1024*768 크기의 윈도우 창 생성
	// SDL에서 SDL_Window*는 "창 핸들(handle)" 같은 존재.
	// 창을 만들면 OS 자원을 잡기 때문에,
	// 마지막에 반드시 SDL_DestroyWindow()로 해제해야 함.
	m_window = SDL_CreateWindow("My SDL3 Game", 1024, 768, 0);
	if (!m_window)
	{
		return false;
	}

	m_renderer = SDL_CreateRenderer(m_window, nullptr);
	if (!m_renderer)
	{
		return false;
	}

    m_renderer2D.SetNative(m_renderer); //Render클래스에서 SDL_Renderer객체 관리
    m_Time.Reset();
	m_running = true;
	return true;
}

void Application::SetScene(std::unique_ptr<Scene> next) {
    // 즉시 OnExit/OnEnter 하지 말고, 예약만 한다.
    m_pendingScene = std::move(next);
}

void Application::PumpEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) {
            m_running = false;
            return;
        }

        // Input에 이벤트 반영
        m_Input.ProcessEvent(e);

        if (m_scene) m_scene->HandleEvent(*this, e);
    }
}

void Application::CommitSceneChangeIfNeeded()
{
    if (!m_pendingScene) return;

    if (m_scene) m_scene->OnExit(*this);

    m_scene = std::move(m_pendingScene); //std::move가 실행되는 순간 자동으로 비워지면서 nullptr 상태

    if (m_scene) m_scene->OnEnter(*this);

    // 씬 전환 직후 dt 튐 방지
    m_Time.Reset();
}

void Application::Run() { //게임 루프
    CommitSceneChangeIfNeeded();

    while (m_running) { //루프가 한번 돌때가 (1 Frame)
        m_Input.BeginFrame();
        PumpEvents();
        m_Time.Tick();
        const float dt = m_Time.DeltaSeconds();

        if (m_scene) m_scene->Update(*this, dt);

        // ---- Render ----
        if (m_renderer) {
            auto& r = m_renderer2D;
            r.BackGroundColor({ 0,0,0,255 }); // 배경 지우기(검정)
            if (m_scene) m_scene->Render(*this, r);
            r.EndFrame();
        }

        // 프레임 끝에서 씬 교체 커밋
        CommitSceneChangeIfNeeded();
        SDL_Delay(1);
    }
}

void Application::Shutdown() {
    if (m_shutdownDone) return;
    m_shutdownDone = true;

    if (m_scene) {
        m_scene->OnExit(*this);
        m_scene.reset();
    }

    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
    SDL_Quit();
}