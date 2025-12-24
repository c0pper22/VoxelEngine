#include "Input.h"
#include <GLFW/glfw3.h>
#include <iostream>

bool Input::m_keys[512];
bool Input::m_lastKeys[512];
float Input::m_mouseX = 0.0f;
float Input::m_mouseY = 0.0f;
float Input::m_mouseDX = 0.0f;
float Input::m_mouseDY = 0.0f;
bool Input::m_firstMouse = true;
std::unordered_map<std::string, KeyCode> Input::m_actionMap;

KeyCode GLFWToKeyCode(int key) {
    switch (key) {
    case GLFW_KEY_W: return KeyCode::W;
    case GLFW_KEY_A: return KeyCode::A;
    case GLFW_KEY_S: return KeyCode::S;
    case GLFW_KEY_D: return KeyCode::D;
    case GLFW_KEY_SPACE: return KeyCode::Space;
    case GLFW_KEY_LEFT_SHIFT: return KeyCode::LeftShift;
    case GLFW_KEY_ESCAPE: return KeyCode::Escape;
    case GLFW_KEY_TAB: return KeyCode::Tab;
    case GLFW_KEY_C: return KeyCode::C;
    default: return KeyCode::Unknown;
    }
}

void Input::Init() {
    m_actionMap["MoveForward"] = KeyCode::W;
    m_actionMap["MoveBack"] = KeyCode::S;
    m_actionMap["MoveLeft"] = KeyCode::A;
    m_actionMap["MoveRight"] = KeyCode::D;
    m_actionMap["Jump"] = KeyCode::Space;
    m_actionMap["Descend"] = KeyCode::LeftShift;
    m_actionMap["ToggleCreative"] = KeyCode::C;
    m_actionMap["Attack"] = KeyCode::MouseLeft;
    m_actionMap["Use"] = KeyCode::MouseRight;
    m_actionMap["Menu"] = KeyCode::Tab;
}

void Input::Update() {
    for (int i = 0; i < 512; i++) {
        m_lastKeys[i] = m_keys[i];
    }

    m_mouseDX = 0.0f;
    m_mouseDY = 0.0f;
}

void Input::SetKey(int glfwKey, bool pressed) {
    KeyCode key = GLFWToKeyCode(glfwKey);
    if (key != KeyCode::Unknown) {
        m_keys[static_cast<int>(key)] = pressed;
    }
}

void Input::SetMouseButton(int glfwButton, bool pressed) {
    if (glfwButton == GLFW_MOUSE_BUTTON_LEFT)
        m_keys[static_cast<int>(KeyCode::MouseLeft)] = pressed;
    if (glfwButton == GLFW_MOUSE_BUTTON_RIGHT)
        m_keys[static_cast<int>(KeyCode::MouseRight)] = pressed;
}

void Input::SetMousePosition(float x, float y) {
    if (m_firstMouse) {
        m_mouseX = x;
        m_mouseY = y;
        m_firstMouse = false;
    }

    m_mouseDX = x - m_mouseX;
    m_mouseDY = m_mouseY - y;

    m_mouseX = x;
    m_mouseY = y;
}

bool Input::IsKeyHeld(KeyCode key) {
    return m_keys[static_cast<int>(key)];
}

bool Input::IsKeyPressed(KeyCode key) {
    int i = static_cast<int>(key);
    return m_keys[i] && !m_lastKeys[i];
}

bool Input::GetButton(const std::string& action) {
    if (m_actionMap.find(action) != m_actionMap.end()) {
        return IsKeyHeld(m_actionMap[action]);
    }
    return false;
}

bool Input::GetButtonDown(const std::string& action) {
    if (m_actionMap.find(action) != m_actionMap.end()) {
        return IsKeyPressed(m_actionMap[action]);
    }
    return false;
}

glm::vec2 Input::GetMousePosition() {
    return glm::vec2(m_mouseX, m_mouseY);
}

glm::vec2 Input::GetMouseDelta() {
    return glm::vec2(m_mouseDX, m_mouseDY);
}