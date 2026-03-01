#pragma once
#include <SDL3/SDL.h>
#include <memory>

class Texture { // Texture는 SDL_Texture*를 "안전하게 관리"하는 RAII 래퍼 클래스.
public:
    explicit Texture(SDL_Texture* tex = nullptr) : m_tex(tex) {} // SDL_Texture*를 받아서 보관
    ~Texture() { 
        if (m_tex) SDL_DestroyTexture(m_tex); 
    } //텍스쳐 리소스 해제

    // 복사 생성/대입을 막는다.
    // 이유:
    // - SDL_Texture*는 "소유권이 하나"여야 안전함.
    // - 복사가 허용되면 두 객체가 같은 SDL_Texture*를 들고 있다가
    //   둘 다 소멸하면서 SDL_DestroyTexture 2번 호출(=크래시) 가능.
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    // 이동 생성자: 다른 Texture가 가진 포인터를 "훔쳐온다"
    // - other.m_tex를 내 m_tex로 가져오고
    // - other.m_tex는 nullptr로 만들어서 "더 이상 소유하지 않게" 함
    Texture(Texture&& other) noexcept : m_tex(other.m_tex) { other.m_tex = nullptr; }

    // 이동 대입: 기존 내가 가진 리소스 해제 -> 상대 리소스 훔치기
    Texture& operator=(Texture&& other) noexcept {
        if (this != &other) {
            if (m_tex) SDL_DestroyTexture(m_tex);
            m_tex = other.m_tex;
            other.m_tex = nullptr;
        }
        return *this;
    }

    // 내부 SDL_Texture*를 "읽기 전용으로" 꺼내는 함수
    // - 렌더러가 SDL_RenderTexture 호출할 때 필요
    SDL_Texture* Native() const { return m_tex; }

    // 텍스처가 유효한지(빈 텍스처인지) 체크용
    bool Valid() const { return m_tex != nullptr; }

private:
    SDL_Texture* m_tex = nullptr; // 실제 SDL 리소스 포인터(이게 진짜 "GPU 텍스처 핸들" 역할)
};

// 게임 로직/씬 쪽에서는 Texture를 직접 소유(값으로 들고)하지 않고,
// shared_ptr로 공유하도록 "핸들 타입"을 만든다.
// 이유:
// - 여러 Scene/오브젝트가 같은 텍스처를 공유하기 쉽고
// - 참조가 모두 사라지면 자동으로 텍스처가 파괴됨(=자동 해제)
using TextureHandle = std::shared_ptr<Texture>;