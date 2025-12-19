#include "World.h"

Chunk* World::getChunk(int x, int z) {
    int chunkX = x / Chunk::CHUNK_SIZE;
    int chunkZ = z / Chunk::CHUNK_SIZE;

    if (x < 0) chunkX = (x - Chunk::CHUNK_SIZE + 1) / Chunk::CHUNK_SIZE;
    if (z < 0) chunkZ = (z - Chunk::CHUNK_SIZE + 1) / Chunk::CHUNK_SIZE;

    for (auto& chunk : chunks) {
        if (chunk->chunkX == chunkX && chunk->chunkZ == chunkZ) {
            return chunk.get();
        }
    }
    return nullptr;
}

World::World()
{

}

void World::draw(Shader& shader)
{
    for (const auto& chunk : chunks)
    {
        chunk->drawOpaque(shader);
    }

    for (const auto& chunk : chunks)
    {
        chunk->drawTransparent(shader);
    }
}

void World::update(glm::vec3 playerPosition)
{
    // Calculate which chunk the player is currently standing in
    int playerChunkX = static_cast<int>(std::floor(playerPosition.x / Chunk::CHUNK_SIZE));
    int playerChunkZ = static_cast<int>(std::floor(playerPosition.z / Chunk::CHUNK_SIZE));

    // --- STEP 1: Unload chunks that are too far away ---
    chunks.erase(
        std::remove_if(chunks.begin(), chunks.end(), [&](const std::unique_ptr<Chunk>& c) {
            int dist = std::max(std::abs(c->chunkX - playerChunkX), std::abs(c->chunkZ - playerChunkZ));
            return dist > renderDistance + 1;
            }),
        chunks.end()
    );

    // --- STEP 2: Load new chunks ---
    int chunksLoaded = 0;

    for (int x = playerChunkX - renderDistance; x <= playerChunkX + renderDistance; x++) {
        for (int z = playerChunkZ - renderDistance; z <= playerChunkZ + renderDistance; z++) {

            // Check if chunk exists
            bool exists = false;
            for (auto& chunk : chunks) {
                if (chunk->chunkX == x && chunk->chunkZ == z) {
                    exists = true;
                    break;
                }
            }

            if (!exists) {
                if (chunksLoaded < chunksToLoadPerFrame) {
                    auto newChunk = std::make_unique<Chunk>(this, x, z);

                    // 1. Generate the new chunk's mesh
                    newChunk->update();

                    // 2. Add to list
                    chunks.push_back(std::move(newChunk));
                    chunksLoaded++;

                    // 3. Convert chunk coords to world coords
                    int worldX = x * Chunk::CHUNK_SIZE;
                    int worldZ = z * Chunk::CHUNK_SIZE;

                    if (Chunk* neighbor = getChunk(worldX - Chunk::CHUNK_SIZE, worldZ)) neighbor->update();
                    if (Chunk* neighbor = getChunk(worldX + Chunk::CHUNK_SIZE, worldZ)) neighbor->update();
                    if (Chunk* neighbor = getChunk(worldX, worldZ - Chunk::CHUNK_SIZE)) neighbor->update();
                    if (Chunk* neighbor = getChunk(worldX, worldZ + Chunk::CHUNK_SIZE)) neighbor->update();
                }
            }
        }
    }
}

uint8_t World::getBlock(int x, int y, int z) {
    Chunk* chunk = getChunk(x, z);
    if (!chunk) return 0; 

    int localX = x % Chunk::CHUNK_SIZE;
    int localZ = z % Chunk::CHUNK_SIZE;

    if (localX < 0) localX += Chunk::CHUNK_SIZE;
    if (localZ < 0) localZ += Chunk::CHUNK_SIZE;

    return chunk->getBlock(localX, y, localZ);
}

void World::setBlock(int x, int y, int z, uint8_t type) {
    Chunk* chunk = getChunk(x, z);
    if (!chunk) return;

    int localX = x % Chunk::CHUNK_SIZE;
    int localZ = z % Chunk::CHUNK_SIZE;

    if (localX < 0) localX += Chunk::CHUNK_SIZE;
    if (localZ < 0) localZ += Chunk::CHUNK_SIZE;

    chunk->setBlock(localX, y, localZ, type);
    chunk->update();

    // --- Update Neighbors if on edge ---
    if (localX == 0) {
        Chunk* neighbor = getChunk(x - 1, z);
        if (neighbor) neighbor->update();
    }
    if (localX == Chunk::CHUNK_SIZE - 1) {
        Chunk* neighbor = getChunk(x + 1, z);
        if (neighbor) neighbor->update();
    }
    // Same for Z axis
    if (localZ == 0) {
        Chunk* neighbor = getChunk(x, z - 1);
        if (neighbor) neighbor->update();
    }
    if (localZ == Chunk::CHUNK_SIZE - 1) {
        Chunk* neighbor = getChunk(x, z + 1);
        if (neighbor) neighbor->update();
    }
}
RayHit World::RayCast(glm::vec3 start, glm::vec3 dir, float maxDist) {
    // 1. Setup DDA variables
    glm::ivec3 mapPos = glm::floor(start); // Current voxel coord
    glm::vec3 deltaDist;
    glm::vec3 sideDist;
    glm::ivec3 step;
    glm::ivec3 rayNormal = glm::ivec3(0);

    // Calculate length of ray to travel 1 unit in each axis
    // If dir is 0, set to infinity to avoid division by zero
    deltaDist.x = (dir.x == 0) ? 1e30 : std::abs(1.0f / dir.x);
    deltaDist.y = (dir.y == 0) ? 1e30 : std::abs(1.0f / dir.y);
    deltaDist.z = (dir.z == 0) ? 1e30 : std::abs(1.0f / dir.z);

    // Calculate step and initial sideDist
    if (dir.x < 0) {
        step.x = -1;
        sideDist.x = (start.x - mapPos.x) * deltaDist.x;
    }
    else {
        step.x = 1;
        sideDist.x = (mapPos.x + 1.0f - start.x) * deltaDist.x;
    }

    if (dir.y < 0) {
        step.y = -1;
        sideDist.y = (start.y - mapPos.y) * deltaDist.y;
    }
    else {
        step.y = 1;
        sideDist.y = (mapPos.y + 1.0f - start.y) * deltaDist.y;
    }

    if (dir.z < 0) {
        step.z = -1;
        sideDist.z = (start.z - mapPos.z) * deltaDist.z;
    }
    else {
        step.z = 1;
        sideDist.z = (mapPos.z + 1.0f - start.z) * deltaDist.z;
    }

    // 2. Perform DDA Step
    float dist = 0.0f;
    while (dist < maxDist) {
        // Walk along the shortest path
        if (sideDist.x < sideDist.y && sideDist.x < sideDist.z) {
            sideDist.x += deltaDist.x;
            mapPos.x += step.x;
            dist = sideDist.x - deltaDist.x; // Actual distance traveled
            rayNormal = glm::ivec3(-step.x, 0, 0); // We hit a face on the X axis
        }
        else if (sideDist.y < sideDist.z) {
            sideDist.y += deltaDist.y;
            mapPos.y += step.y;
            dist = sideDist.y - deltaDist.y;
            rayNormal = glm::ivec3(0, -step.y, 0); // We hit a face on the Y axis
        }
        else {
            sideDist.z += deltaDist.z;
            mapPos.z += step.z;
            dist = sideDist.z - deltaDist.z;
            rayNormal = glm::ivec3(0, 0, -step.z); // We hit a face on the Z axis
        }

        // 3. Check for block hit
        if (getBlock(mapPos.x, mapPos.y, mapPos.z) != 0) { // 0 is AIR
            return { true, mapPos, rayNormal };
        }
    }

    return { false, glm::ivec3(0), glm::ivec3(0) };
}
