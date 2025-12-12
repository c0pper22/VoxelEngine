#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>

#include "Mesh.h"

namespace VERTICES {
    inline constexpr std::array<Vertex, 6> TOP_FACE = {{
        { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f) }, // TOP Y+ FACE
        { glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f) },
        { glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f) },
        { glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f) },
        { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 0.0f) },
        { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f) }
    }};

    inline constexpr std::array<Vertex, 6> BOTTOM_FACE = {{
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) }, // BOTTOM -Y FACE
        { glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 1.0f) },
        { glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f) },
        { glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f) },
        { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },
    }};

    inline constexpr std::array<Vertex, 6> RIGHT_FACE = {{
        { glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f) }, // RIGHT X+ FACE
        { glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f) },
        { glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f) },
        { glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f) },
        { glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f) },
        { glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f) },
    }};

    inline constexpr std::array<Vertex, 6> LEFT_FACE = {{
        { glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f) }, // LEFT X- FACE
        { glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f) },
        { glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f) },
        { glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f) },
    }};

    inline constexpr std::array<Vertex, 6> BACK_FACE = {{
        { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f) }, // BACK Z- FACE
        { glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f) },
        { glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 1.0f) },
        { glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 1.0f) },
        { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 1.0f) },
        { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f) },
    }};

    inline constexpr std::array<Vertex, 6> FRONT_FACE = {{
        { glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 0.0f) }, // FRONT Z+ FACE
        { glm::vec3( 0.5f, -0.5f, -0.5f),   glm::vec2(1.0f, 0.0f) },
        { glm::vec3( 0.5f,  0.5f, -0.5f),   glm::vec2(1.0f, 1.0f) },
        { glm::vec3( 0.5f,  0.5f, -0.5f),   glm::vec2(1.0f, 1.0f) },
        { glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec2(0.0f, 1.0f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 0.0f) },
    }};
}