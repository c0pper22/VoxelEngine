#pragma once // Added pragma once just in case
#include <glm/glm.hpp>
#include <array>
#include "Mesh.h"

namespace VERTICES {
    // Top Face (Y+)
    inline constexpr std::array<Vertex, 6> TOP_FACE = { {
        { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f) }, // Top Left
        { glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f) }, // Top Right
        { glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f) }, // Bottom Right
        { glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f) }, // Bottom Right
        { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 0.0f) }, // Bottom Left
        { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f) }  // Top Left
    } };

    // Bottom Face (Y-)
    inline constexpr std::array<Vertex, 6> BOTTOM_FACE = { {
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) }, // Top Left
        { glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 1.0f) }, // Top Right
        { glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f) }, // Bottom Right
        { glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f) }, // Bottom Right
        { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f) }, // Bottom Left
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) }, // Top Left
    } };

    // Right Face (X+)
    inline constexpr std::array<Vertex, 6> RIGHT_FACE = { {
        { glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f) }, // Bottom Left
        { glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec2(1.0f, 0.0f) }, // Bottom Right
        { glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f) }, // Top Right
        { glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f) }, // Top Right
        { glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec2(0.0f, 1.0f) }, // Top Left
        { glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f) }, // Bottom Left
    } };

    // Left Face (X-)
    inline constexpr std::array<Vertex, 6> LEFT_FACE = { {
        { glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 0.0f) }, // Bottom Left
        { glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec2(1.0f, 0.0f) }, // Bottom Right
        { glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 1.0f) }, // Top Right
        { glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 1.0f) }, // Top Right
        { glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec2(0.0f, 1.0f) }, // Top Left
        { glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 0.0f) }, // Bottom Left
    } };

    // Back Face (Z+)
    inline constexpr std::array<Vertex, 6> BACK_FACE = { {
        { glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f) }, // Bottom Left
        { glm::vec3( 0.5f, -0.5f,  0.5f),  glm::vec2(1.0f, 0.0f) }, // Bottom Right
        { glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 1.0f) }, // Top Right
        { glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 1.0f) }, // Top Right
        { glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec2(0.0f, 1.0f) }, // Top Left
        { glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f) }, // Bottom Left
    } };

    // Front Face (Z-)
    inline constexpr std::array<Vertex, 6> FRONT_FACE = { {
        { glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f) }, // Bottom Left
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f) }, // Bottom Right
        { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f) }, // Top Right
        { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f) }, // Top Right
        { glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f) }, // Top Left
        { glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f) }, // Bottom Left
    } };
}