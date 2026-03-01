#pragma once
#include <SDL3/SDL.h>

class Application;

struct Color {
	Uint8 r = 0, g = 0, b = 0, a = 255;
};

class Renderer {
public:
	Renderer() = default;
	explicit Renderer(SDL_Renderer* r) : m_r(r) {}

	void SetNative(SDL_Renderer* r) { m_r = r; }

	// 프레임
	void BackGroundColor(Color clear = { 0,0,0,255 }) {
		SDL_SetRenderDrawColor(m_r, clear.r, clear.g, clear.b, clear.a);
		SDL_RenderClear(m_r);
	}
	void EndFrame() { SDL_RenderPresent(m_r); }

	//간단한 사각형 그리기 로직.
	void DrawRect(const SDL_FRect& rect, Color c) {
		SDL_SetRenderDrawColor(m_r, c.r, c.g, c.b, c.a);
		SDL_RenderRect(m_r, &rect);
	}
	void DrawFillRect(const SDL_FRect& rect, Color c) {
		SDL_SetRenderDrawColor(m_r, c.r, c.g, c.b, c.a);
		SDL_RenderFillRect(m_r, &rect);
	}

	// 텍스처는 다음 단계(ResourceManager)에서 확장
	void DrawTexture(SDL_Texture* tex, const SDL_FRect& dst, const SDL_FRect* src = nullptr) {
		SDL_RenderTexture(m_r, tex, src, &dst);
	}

private:
	SDL_Renderer* m_r = nullptr;
};