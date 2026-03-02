#pragma once
#include <SDL3/SDL.h>

// 2D 최소 트랜스폼.
// z는 DrawQueue 정렬에 직결되므로 여기서 관리하는 게 자연스럽다.
struct Transform2D {
    SDL_FPoint position{ 0.f, 0.f };
    SDL_FPoint scale{ 1.f, 1.f };
    float rotation = 0.f;     // degrees 기준(네 DrawCommand rotation과 동일)
    int   z = 0;              // 낮을수록 뒤(먼저 그려짐)
};