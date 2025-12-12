#pragma once
#include <vector>
#include <memory>
#include "Chunk.h"
#include "Shader.h"

class World
{
private:
    std::vector<std::unique_ptr<Chunk>> chunks;

public:
    World();
    void draw(Shader& shader);
};