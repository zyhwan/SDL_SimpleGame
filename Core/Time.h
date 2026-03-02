#pragma once
#include<SDL3/SDL.h>

//디바이스, 하드웨어의 기능에 따라 Framelate가 달라지기 때문에 동일한 시간타임을 적용하기 위한 클래스.
class Time {
public:
    void Reset() {
        m_lastCounter = SDL_GetPerformanceCounter();
        m_deltaRawSeconds = 0.0f;
        m_deltaUnscaledSeconds = 0.0f;
        m_deltaSeconds = 0.0f;
    }

    void Tick() {
        const Uint64 now = SDL_GetPerformanceCounter();
        const Uint64 freq = SDL_GetPerformanceFrequency();

        const Uint64 diff = now - m_lastCounter;
        m_lastCounter = now;

        // 1) raw dt (측정값)
        m_deltaRawSeconds = (freq > 0)
            ? (static_cast<float>(diff) / static_cast<float>(freq))
            : 0.0f;

        // 2) clamp 적용
        float clamped = m_deltaRawSeconds;
        if (clamped > m_maxDeltaSeconds) clamped = m_maxDeltaSeconds;
        if (clamped < 0.0f) clamped = 0.0f;

        // 3) unscaled dt (UI/메뉴는 이걸 사용)
        m_deltaUnscaledSeconds = clamped;

        // 4) scaled dt (게임 시뮬은 이걸 사용)
        float scaled = clamped * m_timeScale;
        if (scaled < 0.0f) scaled = 0.0f;
        m_deltaSeconds = scaled;
    }

    // --- dt getters ---
    float DeltaSeconds() const { return m_deltaSeconds; }              // 스케일 적용 후(게임 시뮬)
    float DeltaSecondsUnscaled() const { return m_deltaUnscaledSeconds; } // 스케일 적용 전(UI/메뉴)
    float DeltaSecondsRaw() const { return m_deltaRawSeconds; }        // 클램프 전(디버그)

    // --- dt clamp ---
    void SetMaxDeltaSeconds(float v) { m_maxDeltaSeconds = (v > 0.0f) ? v : 0.0f; }
    float GetMaxDeltaSeconds() const { return m_maxDeltaSeconds; }

    // --- time scale ---
    void SetTimeScale(float s) { m_timeScale = (s >= 0.0f) ? s : 0.0f; }
    float GetTimeScale() const { return m_timeScale; }

private:
    Uint64 m_lastCounter = 0;

    float  m_deltaRawSeconds = 0.0f;       // 측정값(클램프 전)
    float  m_deltaUnscaledSeconds = 0.0f;  // 클램프 후, 스케일 전
    float  m_deltaSeconds = 0.0f;          // 클램프 후, 스케일 후

    float  m_maxDeltaSeconds = 0.05f;      // 기본 50ms clamp
    float  m_timeScale = 1.0f;             // 1=정상, 0=정지
};