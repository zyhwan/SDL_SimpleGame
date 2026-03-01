#pragma once
#include<memory>
#include<SDL3/SDL.h>

class Scene; //전방 선언

class Application {
public:
	Application();
	~Application();

	bool Init();
	void Run();
	void Shutdown();

	//씬 교체
	void SetScene(std::unique_ptr<Scene> next);

	SDL_Window* GetWindow() const { return m_window; }
	SDL_Renderer* GetRenderer() const { return m_renderer; }

	void quit() { m_running = false; }
private:
	void PumpEvents();
private:
	bool m_shutdownDone = false; //shutdown여부
	bool m_running = false; //동작 여부

	SDL_Window* m_window = nullptr; //윈도우 객체
	SDL_Renderer* m_renderer = nullptr; //렌더러

	std::unique_ptr<Scene> m_scene; //현재 씬의 정보
};