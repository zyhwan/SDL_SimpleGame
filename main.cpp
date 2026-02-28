#include<iostream>
#include <SDL3/SDL.h> //SDL 헤더로 선언만 되어 있고 정의는 lib에 되어 있음. 그래서 lib를 무조건 호출해야함.
#include <SDL3/SDL_main.h>

#pragma comment(lib, "SDL3")	// 반드시 SDL3.lib 링킹 <- 비쥬얼 스튜디오가 

using namespace std;

int SDL_main(int argc, char* argv[])
{
	cout << "SDL 간단 프로젝트 시작!" << endl;

	// SDL 초기화
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	// 1024*768 크기의 윈도우 창 생성
	SDL_Window* MyWindow = SDL_CreateWindow("Test", 1024, 768, SDL_WINDOW_OPENGL);

	// 무한 루프
	bool running = true;
	while (running) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_EVENT_QUIT) {
				running = false;
			}
		}

		SDL_Delay(16); // CPU 100% 방지
	}

	// 생성한 창 메모리에서 해제
	SDL_DestroyWindow(MyWindow);

	// SDL에서 사용한 모든 시스템 리소스를 해제하고 종료
	SDL_Quit();

	return 0;
}