#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <cstdint>

struct Color {
    Uint8 r = 0, g = 0, b = 0, a = 255;
};

enum class DrawType : uint8_t {
    RectOutline,
    RectFilled,
    Texture
};

// 사각형/텍스처 모두 통일해서 z 정렬 가능하게 "한 커맨드"로 묶음
struct DrawCommand {
    DrawType type = DrawType::RectOutline;
    int      z = 0;               // 낮을수록 먼저 그려짐(=뒤 레이어)
    Color    color{ 255,255,255,255 };

    // Rect 명령
    SDL_FRect rect{ 0,0,0,0 };

    // Texture 명령
    SDL_Texture* texture = nullptr;
    SDL_FRect    dst{ 0,0,0,0 };
    SDL_FRect    src{ 0,0,0,0 };
    bool         useSrc = false;

    // (선택) 회전/플립 등 확장 포인트
    float rotation = 0.0f;
    SDL_FPoint origin{ 0.0f, 0.0f };
    SDL_FlipMode flip = SDL_FLIP_NONE;
};

class DrawQueue {
public:
    void Clear() { m_cmds.clear(); }

    void AddRectOutline(const SDL_FRect& r, Color c, int z = 0) {
        DrawCommand cmd;
        cmd.type = DrawType::RectOutline;
        cmd.rect = r;
        cmd.color = c;
        cmd.z = z;
        m_cmds.push_back(cmd);
    }

    void AddRectFilled(const SDL_FRect& r, Color c, int z = 0) {
        DrawCommand cmd;
        cmd.type = DrawType::RectFilled;
        cmd.rect = r;
        cmd.color = c;
        cmd.z = z;
        m_cmds.push_back(cmd);
    }

    void AddTexture(SDL_Texture* tex, const SDL_FRect& dst, int z = 0) {
        DrawCommand cmd;
        cmd.type = DrawType::Texture;
        cmd.texture = tex;
        cmd.dst = dst;
        cmd.z = z;
        m_cmds.push_back(cmd);
    }

    void AddTexture(SDL_Texture* tex, const SDL_FRect& dst, const SDL_FRect& src, int z = 0) {
        DrawCommand cmd;
        cmd.type = DrawType::Texture;
        cmd.texture = tex;
        cmd.dst = dst;
        cmd.src = src;
        cmd.useSrc = true;
        cmd.z = z;
        m_cmds.push_back(cmd);
    }

    const std::vector<DrawCommand>& Commands() const { return m_cmds; }
    std::vector<DrawCommand>& Commands() { return m_cmds; }

private:
    std::vector<DrawCommand> m_cmds;
};