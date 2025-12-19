#pragma once
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Texture.h"

class Crosshair
{
private:
    float quadVertices[24] = {
        0.0f, 1.0f,   0.0f, 1.0f,
        1.0f, 0.0f,   1.0f, 0.0f,
        0.0f, 0.0f,   0.0f, 0.0f,

        0.0f, 1.0f,   0.0f, 1.0f,
        1.0f, 1.0f,   1.0f, 1.0f,
        1.0f, 0.0f,   1.0f, 0.0f
    };
    unsigned int VAO, VBO;
    std::unique_ptr<Texture> texture;
public:
    Crosshair();

    void draw(Shader& shader, int SCR_WIDTH, int SCR_HEIGHT);
};