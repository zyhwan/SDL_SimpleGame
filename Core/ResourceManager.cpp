#include "ResourceManager.h"

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

    // 3) 디스크에서 이미지 로드
    //    (BMP로 시작하면 SDL_LoadBMP가 가장 단순)
    SDL_Surface* surf = SDL_LoadBMP(path.c_str());
    if (!surf) {
        SDL_Log("SDL_LoadBMP failed (%s): %s", path.c_str(), SDL_GetError());
        return nullptr;
    }

    // 4) Surface -> Texture 변환(여기서 GPU 텍스처 생성)
    SDL_Texture* raw = SDL_CreateTextureFromSurface(m_renderer, surf);
    SDL_DestroySurface(surf); // 5) Surface는 CPU 메모리라서 변환 후 해제

    if (!raw) {
        SDL_Log("CreateTextureFromSurface failed (%s): %s", path.c_str(), SDL_GetError());
        return nullptr;
    }

    // 6) raw(SDL_Texture*)를 Texture RAII로 감싼다.
    //    - 여기서부터는 Texture가 소멸 시 DestroyTexture를 책임짐
    auto tex = std::make_shared<Texture>(raw);

    // 7) 캐시에 저장(weak_ptr)
    //    - 캐시는 tex의 생명주기를 "붙잡지 않음"
    //    - 사용처가 shared_ptr 들고 있을 때만 살아있음
    m_texCache[path] = tex; // weak_ptr로 저장
    // 8) 사용처(Scene 등)에게 shared_ptr 반환
    return tex;
}

void ResourceManager::Clear()
{

}