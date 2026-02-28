#include "Application.h"

Application::Application() = default;


Application::~Application()
{
	Shutdown();
}

bool Application::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_Log("SDL_Init failed: %s", SDL_GetError);
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

	m_running = true;
	return true;
}

void Application::SetScene(std::unique_ptr<Scene> next) {
    if (m_scene) m_scene->OnExit();
    m_scene = std::move(next);
    if (m_scene) m_scene->OnEnter(*this);
}

void Application::PumpEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) {
            m_running = false;
            return;
        }
        if (m_scene) m_scene->HandleEvent(*this, e);
    }
}

void Application::Run() {
    while (m_running) {
        PumpEvents();

        // ---- Update (임시 dt=1/60) ----
        if (m_scene) m_scene->Update(*this, 1.0f / 60.0f);

        // ---- Render ----
        if (m_renderer) {
            // 배경 지우기(검정)
            SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
            SDL_RenderClear(m_renderer);

            if (m_scene) m_scene->Render(*this, m_renderer);

            SDL_RenderPresent(m_renderer);
        }

        SDL_Delay(16);
    }
}

void Application::Shutdown() {
    if (m_scene) {
        m_scene->OnExit();
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