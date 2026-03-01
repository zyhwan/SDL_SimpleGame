#pragma once
#include <SDL3/SDL.h>
#include <algorithm>
#include "DrawCommand.h"

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

    void Flush(DrawQueue& q) {
        auto& cmds = q.Commands(); //타입은 vector

        // z 오름차순 정렬(뒤 -> 앞)
        std::stable_sort(cmds.begin(), cmds.end(), 
            [](const DrawCommand& a, const DrawCommand& b) {
                return a.z < b.z;
            });

        for (const auto& cmd : cmds) {
            switch (cmd.type) {
            case DrawType::RectOutline: { //선만 그리기
                SDL_SetRenderDrawColor(m_r, cmd.color.r, cmd.color.g, cmd.color.b, cmd.color.a);
                SDL_RenderRect(m_r, &cmd.rect);
                break;
            }
            case DrawType::RectFilled: { //사각형 채워서 그리기
                SDL_SetRenderDrawColor(m_r, cmd.color.r, cmd.color.g, cmd.color.b, cmd.color.a);
                SDL_RenderFillRect(m_r, &cmd.rect);
                break;
            }
            case DrawType::Texture: { //텍스쳐 넣기
                if (!cmd.texture) break;
                const SDL_FRect* pSrc = cmd.useSrc ? &cmd.src : nullptr; //스프라이트 애니메이션이면 src를 사용한다는 로직.
                // 회전/플립 확장하려면 SDL_RenderTextureRotated() 계열로 교체
                SDL_RenderTexture(m_r, cmd.texture, pSrc, &cmd.dst);
                break;
            }
            }
        }
        q.Clear(); // 보통 flush 후 clear
    }

private:
	SDL_Renderer* m_r = nullptr;
};