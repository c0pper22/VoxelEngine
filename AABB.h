#pragma once
#include <glm/glm.hpp>

class AABB
{
public:
    glm::vec3 position;
    glm::vec3 dimensions; // x = width, y = height, z = depth

    AABB(glm::vec3 pos, glm::vec3 dim);

    glm::vec3 getMin() const;
    glm::vec3 getMax() const;

    bool collides(const AABB& other) const;
};