#pragma once
#include <glm/glm.hpp>
#include <unordered_map>
#include <string>

enum class KeyCode {
    Unknown,
    W, A, S, D,
    Space, LeftShift, Control,
    Escape, Tab,
    C,
    MouseLeft, MouseRight
};

class Input {
private:
    static bool m_keys[512];
    static bool m_lastKeys[512];
    static bool m_frameKeys[512];

    static float m_mouseX;
    static float m_mouseY;
    static float m_mouseDX;
    static float m_mouseDY;
    static bool m_firstMouse;

    static std::unordered_map<std::string, KeyCode> m_actionMap;

public:
    static void Init();
    static void Update();
    static void ClearPendingKeys();

    static void SetKey(int glfwKey, bool pressed);
    static void SetMouseButton(int glfwButton, bool pressed);
    static void SetMousePosition(float x, float y);


    static bool IsKeyHeld(KeyCode key);
    static bool IsKeyPressed(KeyCode key);

    static bool GetButton(const std::string& action);
    static bool GetButtonDown(const std::string& action);

    static glm::vec2 GetMousePosition();
    static glm::vec2 GetMouseDelta();
};