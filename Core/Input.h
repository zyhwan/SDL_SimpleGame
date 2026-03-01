#pragma once
#include <SDL3/SDL.h>
#include <unordered_set> // 키가 "눌려있는지" 등을 빠르게 조회하기 위해 해시셋 사용

class Input {
public:
    void BeginFrame() { // 프레임이 시작할 때 호출한다.
        m_pressedThisFrame.clear(); // "이번 프레임에 새로 눌린 키/떼어진 키" 기록은 프레임마다 초기화해야 한다.
        m_releasedThisFrame.clear(); // m_down(현재 누르는 중)은 프레임 넘어가도 유지해야 하므로 지우지 않는다.
    }

    void ProcessEvent(const SDL_Event& e) {
        // Input 상태를 갱신 이 함수는 PollEvent로 뽑힌 이벤트마다 호출되는 구조가 보통이다.

        if (e.type == SDL_EVENT_KEY_DOWN && e.key.repeat == 0) { // 키가 눌렸을 때 이벤트
            m_down.insert(e.key.key); 
            m_pressedThisFrame.insert(e.key.key); // 이번 프레임에 새로 눌린 키로 기록
        }
        else if (e.type == SDL_EVENT_KEY_UP) { // 키가 떼어졌을 때 이벤트
            m_down.erase(e.key.key); 
            m_releasedThisFrame.insert(e.key.key); // 이번 프레임에 떼어진 키로 기록
        }
    }

    bool IsDown(SDL_Keycode key) const {
        // "지금 이 순간 눌려있는가?"
        // (키를 누르고 있는 동안 true)
        return m_down.find(key) != m_down.end();
    }

    bool WasPressed(SDL_Keycode key) const {
        // "이번 프레임에 새로 눌렸는가?"
        // BeginFrame() 이후 ~ 현재 프레임 끝 사이에 KeyDown이 한 번이라도 왔으면 true
        return m_pressedThisFrame.find(key) != m_pressedThisFrame.end();
    }

    bool WasReleased(SDL_Keycode key) const {
        // "이번 프레임에 떼어졌는가?"
        return m_releasedThisFrame.find(key) != m_releasedThisFrame.end();
    }

private:
    std::unordered_set<SDL_Keycode> m_down; // 현재 "눌려있는" 키 집합(키를 누르는 동안 유지)
    std::unordered_set<SDL_Keycode> m_pressedThisFrame; // "이번 프레임에 새로 눌린" 키 집합(프레임마다 초기화됨)
    std::unordered_set<SDL_Keycode> m_releasedThisFrame; // "이번 프레임에 떼어진" 키 집합(프레임마다 초기화됨)
};