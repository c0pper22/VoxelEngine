#pragma once
#include "Camera.h"
#include "Window.h"

class Player
{
public:
    Player(Camera* camera);

    void update(float dt);
    void handleInput(Window* window, float dt);

    glm::vec3 getPosition() const;

private:
    Camera* m_camera;
    float moveSpeed = 45.0f;
};