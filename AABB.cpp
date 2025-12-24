#include "AABB.h"

AABB::AABB(glm::vec3 pos, glm::vec3 dim)
    : position(pos), dimensions(dim)
{
}

glm::vec3 AABB::getMin() const
{
    return glm::vec3(
        position.x - dimensions.x / 2.0f,
        position.y,
        position.z - dimensions.z / 2.0f
    );
}

glm::vec3 AABB::getMax() const
{
    return glm::vec3(
        position.x + dimensions.x / 2.0f,
        position.y + dimensions.y,
        position.z + dimensions.z / 2.0f
    );
}

bool AABB::collides(const AABB& other) const
{
    glm::vec3 minA = getMin();
    glm::vec3 maxA = getMax();
    glm::vec3 minB = other.getMin();
    glm::vec3 maxB = other.getMax();

    return (minA.x < maxB.x && maxA.x > minB.x) &&
        (minA.y < maxB.y && maxA.y > minB.y) &&
        (minA.z < maxB.z && maxA.z > minB.z);
}