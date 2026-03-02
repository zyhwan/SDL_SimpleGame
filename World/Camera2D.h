#pragma once
#include <SDL3/SDL.h>

//참고: position을 "카메라 좌상단"으로 정의했다.
struct Camera2D
{
    SDL_FPoint position{ 0.f, 0.f }; // 월드 좌표 기준 카메라 중심(또는 좌상단 기준으로 써도 됨)
    float zoom = 1.f;                // 나중에 확대/축소용(지금은 1로 고정해도 OK)

    // "월드 좌표" -> "스크린 좌표" (좌상단 기준 오프셋만 적용)
    SDL_FPoint WorldToScreenPoint(SDL_FPoint p) const
    {
        return { (p.x - position.x) * zoom, (p.y - position.y) * zoom };
    }

    SDL_FRect WorldToScreenRect(SDL_FRect r) const {
        r.x = (r.x - position.x) * zoom;
        r.y = (r.y - position.y) * zoom;
        r.w = r.w * zoom;
        r.h = r.h * zoom;
        return r;
    }
};