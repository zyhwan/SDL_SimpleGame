#pragma once
#include<memory>
#include<SDL3/SDL.h>
#include"Time.h"
#include"Input.h"
#include"../gfx/Renderer.h"

class Scene; //전방 선언

class Application {
public:
	Application();
	~Application();

	bool Init();
	void Run();
	void Shutdown();

	void SetScene(std::unique_ptr<Scene> next); // 기존 SetScene는 "예약"으로 의미를 바꾼다.

	SDL_Window* GetWindow() const { return m_window; }
	SDL_Renderer* GetRenderer() const { return m_renderer; }

	Time& GetTime() { return m_Time; }
	Input& GetInput() { return m_Input; }

	//렌더링 관리
	Renderer& GetRenderer2D() { return m_renderer2D; } // 새 API

	void quit() { m_running = false; }
private:
	void PumpEvents();
	void CommitSceneChangeIfNeeded(); //루프 마지막에 프레임 끝에서 씬 전환

private:
	bool m_shutdownDone = false; //shutdown여부
	bool m_running = false; //동작 여부

	SDL_Window* m_window = nullptr; //윈도우 객체


	SDL_Renderer* m_renderer = nullptr; //렌더러
	Renderer m_renderer2D;              // wrapper

	Time m_Time;
	Input m_Input;


	std::unique_ptr<Scene> m_scene;        // 현재 씬
	std::unique_ptr<Scene> m_pendingScene; // 다음 씬(예약)
};