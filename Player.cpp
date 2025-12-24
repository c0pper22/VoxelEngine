#include "Player.h"
#include "Input.h"

Player::Player(Camera* camera, World* world, glm::vec3 startPos)
    : m_camera(camera), m_world(world), velocity(0), m_prevPosition(startPos)
{
    boundingBox = std::make_unique<AABB>(startPos, glm::vec3(0.6f, 1.8f, 0.6f));
}

void Player::update(float dt, World& world)
{
    m_prevPosition = boundingBox->position;

    if (creativeMode)
    {
        boundingBox->position = m_camera->Position - glm::vec3(0.0f, 1.6f, 0.0f);
    }
    else
    {
        // === Survival Physics ===

        // 1. Apply Gravity
        velocity.y -= gravity * dt;

        // 2. X Axis Collision
        boundingBox->position.x += velocity.x * dt;
        if (world.checkCollision(*boundingBox)) {
            boundingBox->position.x -= velocity.x * dt;
            velocity.x = 0;
        }

        // 3. Z Axis Collision
        boundingBox->position.z += velocity.z * dt;
        if (world.checkCollision(*boundingBox)) {
            boundingBox->position.z -= velocity.z * dt;
            velocity.z = 0;
        }

        // 4. Y Axis Collision (Ground/Ceiling)
        boundingBox->position.y += velocity.y * dt;
        isGrounded = false;

        if (world.checkCollision(*boundingBox)) {
            boundingBox->position.y -= velocity.y * dt;

            if (velocity.y < 0) isGrounded = true;

            velocity.y = 0;
        }

        m_camera->Position = boundingBox->position + glm::vec3(0.0f, 1.6f, 0.0f);
    }
}

void Player::interpolate(float alpha)
{
    glm::vec3 interpolatedPos = glm::mix(m_prevPosition, boundingBox->position, alpha);

    m_camera->Position = interpolatedPos + glm::vec3(0.0f, 1.6f, 0.0f);
}

void Player::handleInput(float dt)
{
    if (Input::GetButtonDown("ToggleCreative")) {
        creativeMode = !creativeMode;
        velocity = glm::vec3(0.0f);
    }

    // --- MOVEMENT ---
    if (creativeMode)
    {
        if (Input::GetButton("MoveForward")) m_camera->ProcessKeyboard(FORWARD, dt);
        if (Input::GetButton("MoveBack"))    m_camera->ProcessKeyboard(BACKWARD, dt);
        if (Input::GetButton("MoveLeft"))    m_camera->ProcessKeyboard(LEFT, dt);
        if (Input::GetButton("MoveRight"))   m_camera->ProcessKeyboard(RIGHT, dt);
        if (Input::GetButton("Jump"))        m_camera->ProcessKeyboard(UP, dt);
        if (Input::GetButton("Descend"))     m_camera->ProcessKeyboard(DOWN, dt);
    }
    else
    {
        // === SURVIVAL MODE (Walking) ===
        glm::vec3 front = m_camera->Front;
        glm::vec3 right = m_camera->Right;
        front.y = 0;
        right.y = 0;
        front = glm::normalize(front);
        right = glm::normalize(right);

        float currentSpeed = 5.0f;
        glm::vec3 targetVelocity(0.0f);

        if (Input::GetButton("MoveForward")) targetVelocity += front * currentSpeed;
        if (Input::GetButton("MoveBack"))    targetVelocity -= front * currentSpeed;
        if (Input::GetButton("MoveLeft"))    targetVelocity -= right * currentSpeed;
        if (Input::GetButton("MoveRight"))   targetVelocity += right * currentSpeed;

        velocity.x = targetVelocity.x;
        velocity.z = targetVelocity.z;

        if (Input::GetButton("Jump") && isGrounded) {
            velocity.y = jumpForce;
            isGrounded = false;
        }
    }

    // --- MOUSE CLICKING (Breaking/Placing) ---
    if (Input::GetButtonDown("Attack")) {
        RayHit hit = m_world->RayCast(m_camera->Position, m_camera->Front, 8.0f);
        if (hit.hit) {
            m_world->setBlock(hit.position.x, hit.position.y, hit.position.z, 0);
        }
    }

    if (Input::GetButtonDown("Use")) {
        RayHit hit = m_world->RayCast(m_camera->Position, m_camera->Front, 8.0f);
        if (hit.hit) {
            glm::ivec3 p = hit.position + hit.normal;
            if (creativeMode || glm::distance(glm::vec3(p), m_camera->Position) > 1.5f) {
                m_world->setBlock(p.x, p.y, p.z, 6);
            }
        }
    }
}

glm::vec3 Player::getPosition() const
{
    return m_camera->Position;
}