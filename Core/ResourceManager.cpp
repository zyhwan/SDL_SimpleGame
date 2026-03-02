#include "ResourceManager.h"
#include <algorithm>


static bool EndsWithIgnoreCase(const std::string& s, const std::string& suffix)
{
    if (s.size() < suffix.size()) return false;
    auto it1 = s.end() - suffix.size();
    auto it2 = suffix.begin();
    for (; it1 != s.end(); ++it1, ++it2) {
        char a = *it1, b = *it2;
        if ('A' <= a && a <= 'Z') a = char(a - 'A' + 'a');
        if ('A' <= b && b <= 'Z') b = char(b - 'A' + 'a');
        if (a != b) return false;
    }
    return true;
}

TextureHandle ResourceManager::LoadTexture(const std::string& path)
{
    // 1) 캐시에 같은 path가 있나 확인
    auto it = m_texCache.find(path);
    if (it != m_texCache.end()) {
        // 2) weak_ptr을 shared_ptr로 "살려보기"
        //    - 살아있으면(사용중이면) lock()이 shared_ptr을 돌려줌
        //    - 이미 해제됐으면 lock()은 nullptr
        if (auto alive = it->second.lock())
            return alive;

        // 여기로 왔다는 건:
        // - 캐시엔 있었지만 아무도 안 쓰고 있어서 텍스처가 이미 파괴된 상태(expired)
        // - 따라서 다시 로드해야 함
    }

    if (!m_renderer) return nullptr;

    SDL_Texture* raw = nullptr;

    // 3) SDL3_image로 먼저 시도 (PNG/JPG/BMP 등 다 가능)
    // IMG_LoadTexture는 파일 확장자 기반으로 포맷을 처리하고 GPU 텍스처까지 만든다. :contentReference[oaicite:2]{index=2}
    raw = IMG_LoadTexture(m_renderer, path.c_str());
    if (!raw) 
    {
        // 4) 실패하면 BMP 전용 fallback (SDL 기본 로더)
        // SDL_image가 빌드/배포 환경에서 빠져있거나, 지원 코덱이 없을 때 대비
        if (EndsWithIgnoreCase(path, ".bmp")) 
        {
            SDL_Surface* surf = SDL_LoadBMP(path.c_str());
            if (!surf) {
                SDL_Log("SDL_LoadBMP failed (%s): %s", path.c_str(), SDL_GetError());
                return nullptr;
            }

            raw = SDL_CreateTextureFromSurface(m_renderer, surf);
            SDL_DestroySurface(surf);

            if (!raw) {
                SDL_Log("SDL_CreateTextureFromSurface failed (%s): %s", path.c_str(), SDL_GetError());
                return nullptr;
            }
        }
        else 
        {
            return nullptr;
        }
    }

    // 4) RAII 래핑 + 캐시 등록
    auto tex = std::make_shared<Texture>(raw);
    m_texCache[path] = tex;
    return tex;
}

void ResourceManager::Clear()
{
    // weak_ptr 캐시 자체를 비움 (실제 텍스처 생명주기는 shared_ptr이 쥐고 있음)
    m_texCache.clear();
}