#include "Renderer.h"

//한번에 그려주기
void Renderer::Flush(DrawQueue& q)
{
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
            if (!cmd.texture || !cmd.texture->Valid()) break;

            SDL_Texture* raw = cmd.texture->Native();
            const SDL_FRect* pSrc = cmd.useSrc ? &cmd.src : nullptr; //스프라이트 애니메이션 여부.
            SDL_RenderTexture(m_r, raw, pSrc, &cmd.dst);
            break;
        }
        }
    }
    q.Clear(); // 보통 flush 후 clear
}
