#include "World.h"

World::World()
{
    int worldSize = 100;

    for (int x = 0; x < worldSize; x++)
    {
        for (int z = 0; z < worldSize; z++)
        {
            chunks.push_back(std::make_unique<Chunk>(x, z));
        }
    }
}

void World::draw(Shader& shader)
{
    for (const auto& chunk : chunks)
    {
        chunk->draw(shader);
    }
}