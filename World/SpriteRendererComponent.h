#pragma once
#include <SDL3/SDL.h>
#include "../Core/Texture.h"   // TextureHandle
#include "../gfx/DrawCommand.h" // DrawQueue
#include "Component.h"
#include "GameObject.h"

class SpriteRendererComponent : public Component {
public:
    // 텍스처는 Handle로 들고(리소스 매니저 캐시와 공유)
    explicit SpriteRendererComponent(TextureHandle tex = {})
        : m_tex(std::move(tex)) {
    }

    void SetTexture(TextureHandle tex) { m_tex = std::move(tex); }

    // 스프라이트 시트 지원
    void SetSourceRect(const SDL_FRect& src) { m_src = src; m_useSrc = true; }
    void ClearSourceRect() { m_useSrc = false; }

    void SetSize(float w, float h) { m_size = { w, h }; }
    void SetOrigin(float x, float y) { m_origin = { x, y }; }

    void OnRender(World& world, DrawQueue& q) override {
        if (!m_tex || !m_tex->Valid()) return;

        const auto& t = Owner().transform;

        SDL_FRect dst;
        dst.x = t.position.x;
        dst.y = t.position.y;
        dst.w = m_size.x * t.scale.x;
        dst.h = m_size.y * t.scale.y;

        // 카메라 적용(월드 -> 스크린)
        dst = world.Camera().WorldToScreenRect(dst);

        // z는 Transform이 소유, rotation도 Transform과 동일 축 사용
        if (m_useSrc) {
            q.AddTexture(m_tex, dst, m_src, t.z);
            // 회전/원점/플립까지 DrawCommand에 넣고 싶으면
            // DrawQueue.AddTexture가 아니라 Command를 직접 추가하는 API를 하나 더 만들면 깔끔함(아래 참고).
        }
        else {
            q.AddTexture(m_tex, dst, t.z);
        }

        // 현재 DrawQueue::AddTexture는 rotation/origin/flip을 못 받음.
        // 회전까지 쓰려면 DrawQueue에 "AddCommand(DrawCommand)" 같은 저수준 API를 추가하는 게 좋다.
    }

private:
    TextureHandle m_tex;

    SDL_FRect m_src{ 0,0,0,0 };
    bool m_useSrc = false;

    SDL_FPoint m_size{ 64.f, 64.f };   // 기본 스프라이트 크기(임의)
    SDL_FPoint m_origin{ 0.f, 0.f };
};