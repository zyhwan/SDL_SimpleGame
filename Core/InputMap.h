#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <initializer_list>

class Input;

// 한 "행동(Action)"에 여러 키를 바인딩 가능
struct ActionBinding {
    std::vector<SDL_Keycode> keys;
};

// 한 "축(Axis)"에 (음수 방향 키들, 양수 방향 키들) 바인딩
struct AxisBinding {
    std::vector<SDL_Keycode> negative;
    std::vector<SDL_Keycode> positive;
};

class InputMap {
public:
    // ---- Action Binding ----
    void BindAction(const std::string& action, std::initializer_list<SDL_Keycode> keys) {
        m_actions[action].keys.assign(keys.begin(), keys.end());
    }

    bool IsDown(const Input& input, const std::string& action) const;
    bool WasPressed(const Input& input, const std::string& action) const;
    bool WasReleased(const Input& input, const std::string& action) const;

    // ---- Axis Binding ----
    void BindAxis(const std::string& axis,
        std::initializer_list<SDL_Keycode> negative,
        std::initializer_list<SDL_Keycode> positive) {
        AxisBinding b;
        b.negative.assign(negative.begin(), negative.end());
        b.positive.assign(positive.begin(), positive.end());
        m_axes[axis] = std::move(b);
    }

    // -1 ~ +1 (키 2개 동시 누르면 상쇄)
    float Axis(const Input& input, const std::string& axis) const;

private:
    std::unordered_map<std::string, ActionBinding> m_actions;
    std::unordered_map<std::string, AxisBinding>   m_axes;
};