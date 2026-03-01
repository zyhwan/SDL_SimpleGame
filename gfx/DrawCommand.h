#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <cstdint>
#include "../Core/Texture.h" // TextureHandle 사용

struct Color {
    Uint8 r = 0, g = 0, b = 0, a = 255;
};

enum class DrawType {
    RectOutline,
    RectFilled,
    Texture
};

// 사각형/텍스처 모두 통일해서 z 정렬 가능하게 "한 커맨드"로 묶음
struct DrawCommand {
    DrawType type = DrawType::RectOutline;
    int      z = 0;               // 깊이가 낮을수록 먼저 그려짐(=뒤 레이어)
    Color    color{ 255,255,255,255 };

    // Rect 명령
    SDL_FRect rect{ 0,0,0,0 };

    // Texture 명령
    TextureHandle texture; // SDL_Texture* 대신 핸들
    SDL_FRect    dst{ 0,0,0,0 }; //그리는 위치와 어느정도 크기로 그릴지 여부
    SDL_FRect    src{ 0,0,0,0 }; //텍스처 이미지 중에서 어떤 부분만 잘라서 그릴지 지정하는 범위(스프라이트 이미지에서는 필수적)
    bool         useSrc = false;

    //회전/플립 등 확장
    float rotation = 0.0f;
    SDL_FPoint origin{ 0.0f, 0.0f }; //사각형에서 어디를 중심으로 회전시킬지.
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

    void AddTexture(TextureHandle tex, const SDL_FRect& dst, int z = 0) {
        DrawCommand cmd;
        cmd.type = DrawType::Texture;
        cmd.texture = std::move(tex);
        cmd.dst = dst;
        cmd.z = z;
        m_cmds.push_back(cmd);
    }

    void AddTexture(TextureHandle tex, const SDL_FRect& dst, const SDL_FRect& src, int z = 0) {
        DrawCommand cmd;
        cmd.type = DrawType::Texture;
        cmd.texture = std::move(tex);
        cmd.dst = dst;
        cmd.src = src;
        cmd.useSrc = true;
        cmd.z = z;
        m_cmds.push_back(cmd);
    }

    const std::vector<DrawCommand>& Commands() const { return m_cmds; }
    std::vector<DrawCommand>& Commands() { return m_cmds; }

private:
    std::vector<DrawCommand> m_cmds; //한 화면에 쌓이는 렌더들 레이어를 형성함.
};