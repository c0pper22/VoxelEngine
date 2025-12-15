#include "Player.h"
#include <GLFW/glfw3.h> // Needed for GLFW_KEY_W, etc.

Player::Player(Camera* camera)
    : m_camera(camera)
{
    m_camera->MovementSpeed = moveSpeed;
}

void Player::update(float dt)
{
    // Placeholder: In the future, this is where you will apply 
    // gravity and update m_camera->Position based on velocity.
    // For now, the Camera class handles the position directly.
}

void Player::handleInput(Window* window, float dt)
{
    GLFWwindow* glfwWin = window->getGLFWwindow();

    if (glfwGetKey(glfwWin, GLFW_KEY_W) == GLFW_PRESS)
        m_camera->ProcessKeyboard(FORWARD, dt);
    if (glfwGetKey(glfwWin, GLFW_KEY_S) == GLFW_PRESS)
        m_camera->ProcessKeyboard(BACKWARD, dt);
    if (glfwGetKey(glfwWin, GLFW_KEY_A) == GLFW_PRESS)
        m_camera->ProcessKeyboard(LEFT, dt);
    if (glfwGetKey(glfwWin, GLFW_KEY_D) == GLFW_PRESS)
        m_camera->ProcessKeyboard(RIGHT, dt);
    if (glfwGetKey(glfwWin, GLFW_KEY_SPACE) == GLFW_PRESS)
        m_camera->ProcessKeyboard(UP, dt);
    if (glfwGetKey(glfwWin, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        m_camera->ProcessKeyboard(DOWN, dt);
}

glm::vec3 Player::getPosition() const
{
    return m_camera->Position;
}