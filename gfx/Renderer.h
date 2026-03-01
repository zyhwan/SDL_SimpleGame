#pragma once
#include <SDL3/SDL.h>
#include <algorithm>
#include "DrawCommand.h"

struct Color;

class Renderer {
public:
	Renderer() = default;

	void SetNative(SDL_Renderer* r) { m_r = r; }
	SDL_Renderer* Native() const { return m_r; }

	// 배경화면(밑에 깔리는)
	void BackGroundColor(Color clear = { 0,0,0,255 }) {
		SDL_SetRenderDrawColor(m_r, clear.r, clear.g, clear.b, clear.a);
		SDL_RenderClear(m_r);
	}
	void EndFrame() { SDL_RenderPresent(m_r); }

	void Flush(DrawQueue& q);

private:
	SDL_Renderer* m_r = nullptr;
};