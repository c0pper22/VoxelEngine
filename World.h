#pragma once
#include <vector>
#include <memory>
#include "Chunk.h"
#include "Shader.h"

struct RayHit {
    bool hit;
    glm::ivec3 position;
    glm::ivec3 normal;
};

class World
{
private:
    std::vector<std::unique_ptr<Chunk>> chunks;

    int chunksToLoadPerFrame = 4;

    Chunk* getChunk(int x, int z);
public:
    World();
    void draw(Shader& shader);
    void update(glm::vec3 playerPosition);

    uint8_t getBlock(int x, int y, int z);
    void setBlock(int x, int y, int z, uint8_t type);

    RayHit RayCast(glm::vec3 start, glm::vec3 dir, float maxDist);
    int renderDistance = 16;
};