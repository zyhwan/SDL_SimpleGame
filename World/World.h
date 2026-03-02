#pragma once
#include <memory>
#include <vector>
#include <algorithm>
#include "GameObject.h"
#include "Camera2D.h"

class DrawQueue;

// World: 오브젝트의 생명주기/업데이트/렌더를 관리.
// 중요: Spawn/Destroy는 즉시 반영하지 않고 "지연 반영"으로 안정성 확보.
class World {
public:
    GameObject& Spawn() {
        auto obj = std::make_unique<GameObject>();
        GameObject& ref = *obj;
        m_pendingAdd.push_back(std::move(obj));
        return ref;
    }

    void Destroy(GameObject* obj) {
        if (!obj) return;
        m_pendingDestroy.push_back(obj);
    }

    void Update(float dt) {
        CommitPendingAdd();

        for (auto& obj : m_objects) {
            obj->Update(*this, dt);
        }

        CommitPendingDestroy();
    }

    void Render(DrawQueue& q) {
        // Render는 "큐에 쌓기"까지만 담당
        for (auto& obj : m_objects) {
            obj->Render(*this, q);
        }
    }

    void Clear() {
        // 전체 제거 시에도 OnDestroy 호출 일관성 유지
        for (auto& obj : m_objects) obj->OnDestroy(*this);
        m_objects.clear();
        m_pendingAdd.clear();
        m_pendingDestroy.clear();
    }

    //---- 카메라 world가 관리 ----
    Camera2D& Camera() { return m_camera; }
    const Camera2D& Camera() const { return m_camera; }

    void SetCameraPosition(float x, float y) { m_camera.position = { x, y }; }
    SDL_FPoint GetCameraPosition() const { return m_camera.position; }

    Camera2D m_camera{}; //카메라 관리
private:
    void CommitPendingAdd() {
        if (m_pendingAdd.empty()) return;
        for (auto& obj : m_pendingAdd) {
            obj->OnCreate(*this);
            m_objects.push_back(std::move(obj));
        }
        m_pendingAdd.clear();
    }

    void CommitPendingDestroy() {
        if (m_pendingDestroy.empty()) return;

        // m_pendingDestroy에 있는 포인터와 동일한 오브젝트를 찾아 제거
        // 제거 전에 OnDestroy 호출
        for (GameObject* target : m_pendingDestroy) {
            auto it = std::find_if(m_objects.begin(), m_objects.end(),
                [&](const std::unique_ptr<GameObject>& p) { return p.get() == target; });

            if (it != m_objects.end()) {
                (*it)->OnDestroy(*this);
                m_objects.erase(it);
            }
        }

        m_pendingDestroy.clear();
    }

private:
    std::vector<std::unique_ptr<GameObject>> m_objects;
    std::vector<std::unique_ptr<GameObject>> m_pendingAdd;
    std::vector<GameObject*> m_pendingDestroy;
};