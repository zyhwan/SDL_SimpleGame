#pragma once
#include <memory>
#include <vector>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include "Transform2D.h"
#include "Component.h"

class World;
class DrawQueue;

class GameObject {
public:
    Transform2D transform;

    bool Active() const { return m_active; }
    void SetActive(bool v) { m_active = v; }

    // 컴포넌트 추가
    template<typename T, typename... Args>
    T& AddComponent(Args&&... args) {
        static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
        auto ptr = std::make_unique<T>(std::forward<Args>(args)...);
        ptr->SetOwner(this);

        T& ref = *ptr;
        m_components.push_back(std::move(ptr));
        return ref;
    }

    // 간단 GetComponent (최소 구현: O(n))
    template<typename T>
    T* GetComponent() {
        for (auto& c : m_components) {
            if (auto p = dynamic_cast<T*>(c.get())) return p;
        }
        return nullptr;
    }

private:
    friend class World;

    void OnCreate(World& w) {
        for (auto& c : m_components) if (c->Enabled()) c->OnCreate(w);
    }

    void OnDestroy(World& w) {
        for (auto& c : m_components) if (c->Enabled()) c->OnDestroy(w);
    }

    void Update(World& w, float dt) {
        if (!m_active) return;
        for (auto& c : m_components) if (c->Enabled()) c->Update(w, dt);
    }

    void Render(World& w, DrawQueue& q) {
        if (!m_active) return;
        for (auto& c : m_components) if (c->Enabled()) c->OnRender(w, q);
    }

private:
    bool m_active = true;
    std::vector<std::unique_ptr<Component>> m_components;
};