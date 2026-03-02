#pragma once
#include <cstdint>

class GameObject;
class World;
class DrawQueue;

class Component {
public:
    virtual ~Component() = default;

    GameObject& Owner() const { return *m_owner; }
    bool Enabled() const { return m_enabled; }
    void SetEnabled(bool v) { m_enabled = v; }

    // 월드에 붙은 직후 1회 호출(초기화)
    virtual void OnCreate(World& world) {}
    // 월드에서 제거되기 직전 1회 호출(정리)
    virtual void OnDestroy(World& world) {}

    // 매 프레임 로직
    virtual void Update(World& world, float dt) {}

    // 렌더 커맨드 쌓기(핵심)
    virtual void OnRender(World& world, DrawQueue& q) {}

private:
    friend class GameObject;
    void SetOwner(GameObject* o) { m_owner = o; }

    GameObject* m_owner = nullptr;
    bool m_enabled = true;
};