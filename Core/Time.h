#pragma once
#include<SDL3/SDL.h>

//디바이스, 하드웨어의 기능에 따라 Framelate가 달라지기 때문에 동일한 시간타임을 적용하기 위한 클래스.
class Time {
public:
    void Reset() {
        // 기준 시점을 "지금"으로 초기화한다.
        // 게임 시작 직후나 씬 전환 직후 dt가 이상하게 튀는 걸 방지하는 용도.
        m_lastCounter = SDL_GetPerformanceCounter(); //SDL 라이브러리에서 제공하는 고해상도 타이머(Timer) 값을 가져오는 함수
        m_deltaSeconds = 0.0f; // dt(DeltaSeconds)를 0으로 초기화
    }

    void Tick() {
        const Uint64 now = SDL_GetPerformanceCounter();

        // 고해상도 타이머의 주파수(초당 틱 수)를 읽는다.
        // 예: freq = 10,000,000 이면 1초에 1000만 틱.
        const Uint64 freq = SDL_GetPerformanceFrequency(); //고해상도 타이머의 초당 빈도를 가져오는 함수

        // 이번 프레임에서 지난 프레임 이후 얼마나 시간이 흘렀는지(틱 단위)
        const Uint64 diff = now - m_lastCounter;

        // 다음 프레임 계산을 위해 "이전 시점"을 지금으로 갱신
        m_lastCounter = now;

        m_deltaSeconds = (freq > 0) ? (static_cast<float>(diff) / static_cast<float>(freq)) : 0.0f;

        // 갑작스런 디버깅/일시정지로 dt 폭발 방지(선택)
        if (m_deltaSeconds > 0.1f) m_deltaSeconds = 0.1f;
    }

    float DeltaSeconds() const { return m_deltaSeconds; }
private:
    Uint64 m_lastCounter = 0;
    float  m_deltaSeconds = 0.0f;
};