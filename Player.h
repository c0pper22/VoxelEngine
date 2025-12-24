#pragma once
#include "Camera.h"
#include "World.h"
#include "AABB.h"

class Player
{
public:
    Player(Camera* camera, World* world, glm::vec3 startPos);

    void update(float dt, World& world);
    void handleInput(float dt);

    glm::vec3 getPosition() const;
    std::unique_ptr<AABB> boundingBox;

    glm::vec3 velocity;
    bool isGrounded = false;
    bool creativeMode = true;

    // Settings
    float moveSpeed = 100.0f;
    float jumpForce = 10.0f;
    float gravity = 18.0f;
private:
    Camera* m_camera;
    World* m_world;
};