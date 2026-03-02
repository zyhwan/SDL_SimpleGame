#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <unordered_map>
#include <memory>
#include "Texture.h"
#include <SDL3_image/SDL_image.h>

// ResourceManager는 텍스처를 "로드 + 캐시"하는 관리자.
//
// 목표:
// 1) 같은 파일을 여러 번 요청해도 실제 로드는 1번만 하게 만들기
// 2) 여러 곳에서 공유 가능(shared_ptr)
// 3) 더 이상 쓰는 곳이 없으면 자동 해제되게 만들기(weak_ptr 캐시)
class ResourceManager {
public:
    explicit ResourceManager(SDL_Renderer* renderer) : m_renderer(renderer) {} // 생성 시 SDL_Renderer*를 받아둔다.
    void SetRenderer(SDL_Renderer* r) { m_renderer = r; } // 나중에 렌더러가 바뀌는 상황(디바이스 재생성 등)을 대비해 세터 제공

    // 핵심 함수:
    // - path로 텍스처를 요청한다.
    // - 캐시에 있으면 그걸 반환한다.
    // - 없으면 로드해서 캐시에 넣고 반환한다.
    TextureHandle LoadTexture(const std::string& path);

    void Clear();

private:
    // 텍스처 생성에 필요한 SDL 렌더러
    SDL_Renderer* m_renderer = nullptr;

    // 캐시 구조:
    // - key: 파일 경로(path)
    // - value: weak_ptr<Texture>
    //
    // 왜 weak_ptr?
    // - 캐시가 shared_ptr을 들면 "항상 살아있게" 되어 메모리 해제가 안 됨.
    // - weak_ptr로 들면 실제 사용처(Scene/오브젝트)가 shared_ptr을 들고 있을 때만 살아있고,
    //   모두 놓으면 자동으로 텍스처 해제됨.
    std::unordered_map<std::string, std::weak_ptr<Texture>> m_texCache;
};