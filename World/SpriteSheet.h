#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include <cassert>

// 스프라이트 시트의 "프레임 정의 데이터".
// 텍스처는 SpriteRendererComponent가 들고 있어도 되고,
// 여기서 같이 들고 있어도 됨(프로젝트 스타일에 맞춰 선택).
struct SpriteFrame
{
    SDL_Rect src{};        // atlas에서 잘라낼 src rect
    SDL_Point pivot{ 0, 0 }; // (선택) pivot. 없으면 (0,0) 써도 됨
};

class SpriteSheet
{
public:
    SpriteSheet() = default;
    explicit SpriteSheet(std::string name) : m_name(std::move(name)) {}

    void SetName(const std::string& name) { m_name = name; }
    const std::string& GetName() const { return m_name; }

    // 프레임 추가
    int AddFrame(const SDL_Rect& src, SDL_Point pivot = { 0, 0 })
    {
        m_frames.push_back(SpriteFrame{ src, pivot });
        return static_cast<int>(m_frames.size() - 1);
    }

    const SpriteFrame& GetFrame(int index) const
    {
        assert(index >= 0 && index < (int)m_frames.size());
        return m_frames[index];
    }

    int GetFrameCount() const { return static_cast<int>(m_frames.size()); }

private:
    std::string m_name;
    std::vector<SpriteFrame> m_frames;
};