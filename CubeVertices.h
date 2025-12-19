#pragma once 
#include <glm/glm.hpp>
#include <array>
#include "Mesh.h"

namespace VERTICES {

    // Top Face (Y+)
    // Matches input block: "// Top face" (y = 0.5)
    inline constexpr std::array<Vertex, 6> TOP_FACE = { {
        { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f) }, // top-left
        { glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f) }, // bottom-right
        { glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f) }, // top-right     
        { glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f) }, // bottom-right
        { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f) }, // top-left
        { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 0.0f) }  // bottom-left        
    } };

    // Bottom Face (Y-)
    inline constexpr std::array<Vertex, 6> BOTTOM_FACE = { {
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) }, // top-right
        { glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 1.0f) }, // top-left
        { glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f) }, // bottom-left
        { glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f) }, // bottom-left
        { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f) }, // bottom-right
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) }  // top-right
    } };

    // Right Face (X+)
    inline constexpr std::array<Vertex, 6> RIGHT_FACE = { {
        { glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 1.0f) }, // top-left     (Was 1,0)
        { glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 0.0f) }, // bottom-right (Was 0,1)
        { glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec2(0.0f, 1.0f) }, // top-right    (Was 1,1)     
        { glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 0.0f) }, // bottom-right (Was 0,1)
        { glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 1.0f) }, // top-left     (Was 1,0)
        { glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec2(1.0f, 0.0f) }  // bottom-left  (Was 0,0)   
    } };

    // Left Face (X-)
    inline constexpr std::array<Vertex, 6> LEFT_FACE = { {
        { glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 1.0f) }, // top-right   (Fixed & Was 1,0)
        { glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec2(0.0f, 1.0f) }, // top-left    (Was 1,1)
        { glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 0.0f) }, // bottom-left (Was 0,1)
        { glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 0.0f) }, // bottom-left (Was 0,1)
        { glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec2(1.0f, 0.0f) }, // bottom-right(Was 0,0)
        { glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 1.0f) }  // top-right   (Was 1,0)
    } };

    // Back Face (Z-)
    inline constexpr std::array<Vertex, 6> FRONT_FACE = { {
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f) }, // Bottom-left
        { glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f) }, // top-right
        { glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f) }, // bottom-right         
        { glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f) }, // top-right
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f) }, // bottom-left
        { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f) }  // top-left
    } };

    // Front Face (Z+)
    inline constexpr std::array<Vertex, 6> BACK_FACE = { {
        { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f) }, // bottom-left
        { glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f) }, // bottom-right
        { glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 1.0f) }, // top-right
        { glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 1.0f) }, // top-right
        { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 1.0f) }, // top-left
        { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f) }  // bottom-left
    } };
}