#include "Player.h"
#include <GLFW/glfw3.h> // Needed for GLFW_KEY_W, etc.

Player::Player(Camera* camera, World* world)
    : m_camera(camera), m_world(world)
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

    static bool isLeftPressed = false;
    static bool isRightPressed = false;

    // LEFT CLICK: Break Block
    if (glfwGetMouseButton(window->getGLFWwindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (!isLeftPressed) {
            RayHit hit = m_world->RayCast(m_camera->Position, m_camera->Front, 8.0f);
            if (hit.hit) {
                m_world->setBlock(hit.position.x, hit.position.y, hit.position.z, 0); // Set to AIR
            }
            isLeftPressed = true;
        }
    }
    else {
        isLeftPressed = false;
    }

    // RIGHT CLICK: Place Block
    if (glfwGetMouseButton(window->getGLFWwindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        if (!isRightPressed) {
            RayHit hit = m_world->RayCast(m_camera->Position, m_camera->Front, 8.0f);
            if (hit.hit) {
                glm::ivec3 placePos = hit.position + hit.normal;

                glm::vec3 playerPos = m_camera->Position;
                if (glm::distance(glm::vec3(placePos), playerPos) > 1.5f) {
                    m_world->setBlock(placePos.x, placePos.y, placePos.z, 5); // 1 = STONE
                }
            }
            isRightPressed = true;
        }
    }
    else {
        isRightPressed = false;
    }
}

glm::vec3 Player::getPosition() const
{
    return m_camera->Position;
}