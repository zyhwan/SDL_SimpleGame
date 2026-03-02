#include "InputMap.h"
#include "Input.h"

static bool AnyDown(const Input& input, const std::vector<SDL_Keycode>& keys)
{
    for (auto k : keys) if (input.IsDown(k)) return true;
    return false;
}

static bool AnyPressed(const Input& input, const std::vector<SDL_Keycode>& keys)
{
    for (auto k : keys) if (input.WasPressed(k)) return true;
    return false;
}

static bool AnyReleased(const Input& input, const std::vector<SDL_Keycode>& keys)
{
    for (auto k : keys) if (input.WasReleased(k)) return true;
    return false;
}

//-------------------------------------------------------------------------------------------------------------------
bool InputMap::IsDown(const Input& input, const std::string& action) const
{
    auto it = m_actions.find(action);
    if (it == m_actions.end()) return false;
    return AnyDown(input, it->second.keys);
}

bool InputMap::WasPressed(const Input& input, const std::string& action) const
{
    auto it = m_actions.find(action);
    if (it == m_actions.end()) return false;
    return AnyPressed(input, it->second.keys);
}

bool InputMap::WasReleased(const Input& input, const std::string& action) const
{
    auto it = m_actions.find(action);
    if (it == m_actions.end()) return false;
    return AnyReleased(input, it->second.keys);
}

float InputMap::Axis(const Input& input, const std::string& axis) const
{
    auto it = m_axes.find(axis);
    if (it == m_axes.end()) return 0.f;

    const AxisBinding& b = it->second;

    float v = 0.f;
    if (AnyDown(input, b.negative)) v -= 1.f;
    if (AnyDown(input, b.positive)) v += 1.f;
    return v;
}