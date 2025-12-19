#pragma once
#include "Camera.h"
#include "World.h"
#include "Window.h"

class Player
{
public:
    Player(Camera* camera, World* world);

    void update(float dt);
    void handleInput(Window* window, float dt);

    glm::vec3 getPosition() const;

private:
    Camera* m_camera;
    World* m_world;
    float moveSpeed = 45.0f;
};