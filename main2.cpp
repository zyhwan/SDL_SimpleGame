//#include<iostream>
//#include <SDL3/SDL.h> //SDL 헤더로 선언만 되어 있고 정의는 lib에 되어 있음. 그래서 lib를 무조건 호출해야함.
//#include <SDL3/SDL_main.h>
//
//#pragma comment(lib, "SDL3")	// 반드시 SDL3.lib 링킹 <- 비쥬얼 스튜디오가 
//
//using namespace std;
//
//int SDL_main(int argc, char* argv[]) //argc, argv는 커맨드라인 인자(입력값들)
//{
//	cout << "SDL 간단 프로젝트 시작!" << endl;
//
//	// SDL 내부 시스템 초기화
//	// SDL_INIT_VIDEO: 창/그래픽/이벤트(윈도우 관련)를 사용하겠다는 뜻
//	// SDL_INIT_AUDIO: 오디오 시스템 사용하겠다는 뜻
//	// | 는 비트 OR 연산으로 "여러 기능을 동시에 켜겠다"는 플래그 조합 방식.
//	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
//
//	// 1024*768 크기의 윈도우 창 생성
//	// SDL에서 SDL_Window*는 "창 핸들(handle)" 같은 존재.
//	// 창을 만들면 OS 자원을 잡기 때문에,
//	// 마지막에 반드시 SDL_DestroyWindow()로 해제해야 함.
//
//	SDL_Window* MyWindow = SDL_CreateWindow("SDLSimpleGame", 1024, 768, SDL_WINDOW_OPENGL);
//
//	// 무한 루프
//	bool running = true;
//	while (running) {
//		// SDL_Event = 이벤트 하나를 담는 "그릇".
//		// 키보드 / 마우스 / 창 닫기 같은 이벤트가 여기에 들어옴.
//		SDL_Event e;
//
//		// SDL의 이벤트 큐에서 이벤트를 하나씩 꺼내서 e에 채움.
//		// 이벤트가 더 이상 없으면 0을 반환해서 루프 종료.
//		// "이걸 안 하면 창이 멈춘 것처럼 보이는" 이유 :
//		// OS가 보내는 메시지를 받아서 처리해야 창이 살아있음.
//		// SDL_PollEvent가 그 역할(메시지 펌핑)을 수행함.
//		while (SDL_PollEvent(&e)) {
//			if (e.type == SDL_EVENT_QUIT) {
//				running = false;
//			}
//		}
//
//		//현재 스레드를 약 16ms 잠깐 멈춤.
//		//대략 60FPS(1000ms / 16ms ~ 62.5fps) 근처로 루프가 돌도록 "대충" 제한하는 효과.
//		//CPU 100 % 로 미친 듯이 도는 걸 막는 가장 쉬운 방법.
//		SDL_Delay(16); // CPU 100% 방지
//	}
//
//	// 생성한 창 메모리에서 해제
//	SDL_DestroyWindow(MyWindow);
//
//	// SDL에서 사용한 모든 시스템 리소스를 해제하고 종료
//	SDL_Quit();
//
//	return 0;
//}