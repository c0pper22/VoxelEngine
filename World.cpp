#include "World.h"
#include <algorithm>

Chunk* World::getChunk(int x, int z) {
    int chunkX = x / Chunk::CHUNK_SIZE;
    int chunkZ = z / Chunk::CHUNK_SIZE;

    if (x < 0) chunkX = (x - Chunk::CHUNK_SIZE + 1) / Chunk::CHUNK_SIZE;
    if (z < 0) chunkZ = (z - Chunk::CHUNK_SIZE + 1) / Chunk::CHUNK_SIZE;

    ChunkCoord coord = { chunkX, chunkZ };
    auto it = chunks.find(coord);

    if (it != chunks.end()) {
        return it->second.get();
    }
    return nullptr;
}

World::World()
{

}

void World::draw(Shader& shader)
{
    for (const auto& [coord, chunk] : chunks)
    {
        chunk->drawOpaque(shader);
    }

    for (const auto& [coord, chunk] : chunks)
    {
        chunk->drawTransparent(shader);
    }
}

void World::update(glm::vec3 playerPosition, float dt)
{
    int playerChunkX = static_cast<int>(std::floor(playerPosition.x / Chunk::CHUNK_SIZE));
    int playerChunkZ = static_cast<int>(std::floor(playerPosition.z / Chunk::CHUNK_SIZE));

    // --- STEP 1: Unload chunks (Keep existing logic) ---
    for (auto it = chunks.begin(); it != chunks.end(); ) {
        Chunk* c = it->second.get();
        int dist = std::max(std::abs(c->chunkX - playerChunkX), std::abs(c->chunkZ - playerChunkZ));

        if (dist > renderDistance + 1) {
            it = chunks.erase(it);
        }
        else {
            ++it;
        }
    }

    // --- STEP 2: Identify ALL missing chunks first ---
    std::vector<ChunkCoord> chunksToLoad;

    for (int x = playerChunkX - renderDistance; x <= playerChunkX + renderDistance; x++) {
        for (int z = playerChunkZ - renderDistance; z <= playerChunkZ + renderDistance; z++) {
            ChunkCoord coord = { x, z };
            if (chunks.find(coord) == chunks.end()) {
                chunksToLoad.push_back(coord);
            }
        }
    }

    // --- STEP 3: Sort by distance to player ---
    std::sort(chunksToLoad.begin(), chunksToLoad.end(),
        [playerChunkX, playerChunkZ](const ChunkCoord& a, const ChunkCoord& b) {
            int distA = (a.x - playerChunkX) * (a.x - playerChunkX) + (a.z - playerChunkZ) * (a.z - playerChunkZ);
            int distB = (b.x - playerChunkX) * (b.x - playerChunkX) + (b.z - playerChunkZ) * (b.z - playerChunkZ);
            return distA < distB;
        });

    // --- STEP 4: Load the limited batch ---
    int loadedCount = 0;
    for (const auto& coord : chunksToLoad) {
        if (loadedCount >= chunksToLoadPerFrame) break;

        auto newChunk = std::make_unique<Chunk>(this, coord.x, coord.z);

        Chunk* cPtr = newChunk.get();
        chunks[coord] = std::move(newChunk);

        regenerateChunk(cPtr->chunkX, cPtr->chunkZ);

        loadedCount++;

        int worldX = coord.x * Chunk::CHUNK_SIZE;
        int worldZ = coord.z * Chunk::CHUNK_SIZE;
        regenerateChunk((worldX - Chunk::CHUNK_SIZE) / 16, worldZ / 16);
        regenerateChunk((worldX + Chunk::CHUNK_SIZE) / 16, worldZ / 16);
        regenerateChunk(worldX / 16, (worldZ - Chunk::CHUNK_SIZE) / 16);
        regenerateChunk(worldX / 16, (worldZ + Chunk::CHUNK_SIZE) / 16);
    }

    // --- STEP 5: Process Finished Meshes ---
    updateMeshes();
    updateFluids(dt);
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

uint8_t World::getBlockMeta(int x, int y, int z) {
    Chunk* chunk = getChunk(x, z);
    if (!chunk) return 0;

    int localX = x % Chunk::CHUNK_SIZE;
    int localZ = z % Chunk::CHUNK_SIZE;
    if (localX < 0) localX += Chunk::CHUNK_SIZE;
    if (localZ < 0) localZ += Chunk::CHUNK_SIZE;

    return chunk->getMeta(localX, y, localZ);
}

void World::setBlockMeta(int x, int y, int z, uint8_t data) {
    Chunk* chunk = getChunk(x, z);
    if (!chunk) return;

    int localX = x % Chunk::CHUNK_SIZE;
    int localZ = z % Chunk::CHUNK_SIZE;
    if (localX < 0) localX += Chunk::CHUNK_SIZE;
    if (localZ < 0) localZ += Chunk::CHUNK_SIZE;

    chunk->setMeta(localX, y, localZ, data);
    regenerateChunk(chunk->chunkX, chunk->chunkZ);

    // === ADD THIS SECTION ===
    // We must notify neighbors when metadata (water level) changes,
    // otherwise the drying-up chain reaction stops immediately.
    addFluidUpdate(x, y, z);
    addFluidUpdate(x + 1, y, z);
    addFluidUpdate(x - 1, y, z);
    addFluidUpdate(x, y + 1, z);
    addFluidUpdate(x, y - 1, z);
    addFluidUpdate(x, y, z + 1);
    addFluidUpdate(x, y, z - 1);

    // Update neighbor chunks if on the edge
    if (localX == 0) { regenerateChunk(x - 1, z); }
    if (localX == Chunk::CHUNK_SIZE - 1) { regenerateChunk(x + 1, z); }
    if (localZ == 0) { regenerateChunk(x, z - 1); }
    if (localZ == Chunk::CHUNK_SIZE - 1) { regenerateChunk(x, z + 1); }
}

void World::setBlock(int x, int y, int z, uint8_t type) {
    Chunk* chunk = getChunk(x, z);
    if (!chunk) return;

    int localX = x % Chunk::CHUNK_SIZE;
    int localZ = z % Chunk::CHUNK_SIZE;
    if (localX < 0) localX += Chunk::CHUNK_SIZE;
    if (localZ < 0) localZ += Chunk::CHUNK_SIZE;

    chunk->setBlock(localX, y, localZ, type);
    regenerateChunk(chunk->chunkX,chunk->chunkZ);

    addFluidUpdate(x, y, z);
    addFluidUpdate(x + 1, y, z);
    addFluidUpdate(x - 1, y, z);
    addFluidUpdate(x, y + 1, z);
    addFluidUpdate(x, y - 1, z);
    addFluidUpdate(x, y, z + 1);
    addFluidUpdate(x, y, z - 1);

    if (localX == 0) { regenerateChunk(x - 1, z); }
    if (localX == Chunk::CHUNK_SIZE - 1) { regenerateChunk(x + 1, z); }
    if (localZ == 0) { regenerateChunk(x, z - 1); }
    if (localZ == Chunk::CHUNK_SIZE - 1) { regenerateChunk(x, z + 1); }
}

RayHit World::RayCast(glm::vec3 start, glm::vec3 dir, float maxDist) {
    glm::ivec3 mapPos = glm::floor(start);
    glm::vec3 deltaDist;
    glm::vec3 sideDist;
    glm::ivec3 step;
    glm::ivec3 rayNormal = glm::ivec3(0);

    deltaDist.x = (dir.x == 0) ? 1e30 : std::abs(1.0f / dir.x);
    deltaDist.y = (dir.y == 0) ? 1e30 : std::abs(1.0f / dir.y);
    deltaDist.z = (dir.z == 0) ? 1e30 : std::abs(1.0f / dir.z);

    if (dir.x < 0) { step.x = -1; sideDist.x = (start.x - mapPos.x) * deltaDist.x; }
    else { step.x = 1; sideDist.x = (mapPos.x + 1.0f - start.x) * deltaDist.x; }

    if (dir.y < 0) { step.y = -1; sideDist.y = (start.y - mapPos.y) * deltaDist.y; }
    else { step.y = 1; sideDist.y = (mapPos.y + 1.0f - start.y) * deltaDist.y; }

    if (dir.z < 0) { step.z = -1; sideDist.z = (start.z - mapPos.z) * deltaDist.z; }
    else { step.z = 1; sideDist.z = (mapPos.z + 1.0f - start.z) * deltaDist.z; }

    float dist = 0.0f;
    while (dist < maxDist) {
        if (sideDist.x < sideDist.y && sideDist.x < sideDist.z) {
            sideDist.x += deltaDist.x; mapPos.x += step.x; dist = sideDist.x - deltaDist.x; rayNormal = glm::ivec3(-step.x, 0, 0);
        }
        else if (sideDist.y < sideDist.z) {
            sideDist.y += deltaDist.y; mapPos.y += step.y; dist = sideDist.y - deltaDist.y; rayNormal = glm::ivec3(0, -step.y, 0);
        }
        else {
            sideDist.z += deltaDist.z; mapPos.z += step.z; dist = sideDist.z - deltaDist.z; rayNormal = glm::ivec3(0, 0, -step.z);
        }

        uint8_t blockID = getBlock(mapPos.x, mapPos.y, mapPos.z);

        if (blockID != 0 && !BlockRegistry::Get().IsLiquid(blockID)) {
            return { true, mapPos, rayNormal };
        }
    }
    return { false, glm::ivec3(0), glm::ivec3(0) };
}

bool World::isBlockSolid(int x, int y, int z) {
    uint8_t type = getBlock(x, y, z);
    if (type == 0) return false;
    if (BlockRegistry::Get().IsLiquid(type)) return false;
    return true;
}

bool World::checkCollision(const AABB& box) {
    glm::vec3 min = box.getMin();
    glm::vec3 max = box.getMax();

    for (int x = static_cast<int>(std::floor(min.x)); x <= static_cast<int>(std::floor(max.x)); x++) {
        for (int y = static_cast<int>(std::floor(min.y)); y <= static_cast<int>(std::floor(max.y)); y++) {
            for (int z = static_cast<int>(std::floor(min.z)); z <= static_cast<int>(std::floor(max.z)); z++) {
                if (getBlock(x, y, z) != 0 && !BlockRegistry::Get().IsLiquid(getBlock(x, y, z))) return true;
            }
        }
    }
    return false;
}

void World::addFluidUpdate(int x, int y, int z) {
    glm::ivec3 pos(x, y, z);
    if (m_pendingUpdates.find(pos) == m_pendingUpdates.end()) {
        m_pendingUpdates.insert(pos);
        m_fluidQueue.push_back(pos);
    }
}

void World::updateFluids(float dt) {
    m_fluidTimer += dt;
    if (m_fluidTimer < 0.25f) return;
    m_fluidTimer = 0.0f;

    int updatesToProcess = m_fluidQueue.size();
    while (updatesToProcess > 0) {
        glm::ivec3 pos = m_fluidQueue.front();
        m_fluidQueue.pop_front();
        m_pendingUpdates.erase(pos);
        updatesToProcess--;

        uint8_t type = getBlock(pos.x, pos.y, pos.z);
        if (type == WATER_SOURCE || type == WATER_FLOWING) {
            processWater(pos.x, pos.y, pos.z, type);
        }
    }
}

void World::regenerateChunk(int x, int z) {
    Chunk* chunk = getChunk(x * Chunk::CHUNK_SIZE, z * Chunk::CHUNK_SIZE);
    if (!chunk) return;

    bool expected = false;
    if (!chunk->isMeshPending.compare_exchange_strong(expected, true)) return;

    m_threadPool.enqueue([this, chunk, x, z]() {

        ChunkMeshData data = ChunkMesher::GenerateMesh(*chunk);

        {
            std::lock_guard<std::mutex> lock(m_meshMutex);
            m_meshResults.push_back({ x, z, data });
        }
        });
}

void World::updateMeshes() {
    std::lock_guard<std::mutex> lock(m_meshMutex);

    for (const auto& result : m_meshResults) {
        Chunk* c = getChunk(result.chunkX * Chunk::CHUNK_SIZE, result.chunkZ * Chunk::CHUNK_SIZE);
        if (c) {
            c->uploadMesh(result.data.opaqueVertices, result.data.transparentVertices);
        }
    }
    m_meshResults.clear();
}

int World::calculateFlowCost(int x, int y, int z, int initialDepth) {
    // Optimization: Use BFS (Breadth-First Search) instead of recursion.
    // This prevents Stack Overflows and finds the shortest path faster.

    // 1. Constants
    const int MAX_SEARCH_DIST = 4;

    // 2. Queue for BFS: Stores {Position, CurrentDistance}
    std::deque<std::pair<glm::ivec3, int>> queue;
    std::unordered_set<glm::ivec3> visited;

    // Start at the given block with distance 0
    queue.push_back({ {x, y, z}, 0 });
    visited.insert({ x, y, z });

    while (!queue.empty()) {
        // Get the next block to check
        auto [pos, dist] = queue.front();
        queue.pop_front();

        // If we've gone too far, stop checking this path
        if (dist > MAX_SEARCH_DIST) continue;

        // --- 3. CHECK FOR DROP-OFF (The Goal) ---
        uint8_t blockBelow = getBlock(pos.x, pos.y - 1, pos.z);

        // Found a hole (Air below)
        if (blockBelow == AIR) return dist;

        // Found a waterfall (Water that is falling/not solid below)
        if (blockBelow == WATER_FLOWING || blockBelow == WATER_SOURCE) {
            uint8_t blockTwoBelow = getBlock(pos.x, pos.y - 2, pos.z);
            if (!BlockRegistry::Get().IsSolid(blockTwoBelow)) {
                return dist;
            }
        }

        // --- 4. EXPAND TO NEIGHBORS ---
        // If we haven't reached the limit, add neighbors to the queue
        if (dist < MAX_SEARCH_DIST) {
            glm::ivec3 directions[] = { {1,0,0}, {-1,0,0}, {0,0,1}, {0,0,-1} };

            for (const auto& dir : directions) {
                glm::ivec3 nextPos = pos + dir;

                // Don't check the same block twice
                if (visited.find(nextPos) != visited.end()) continue;

                uint8_t nType = getBlock(nextPos.x, nextPos.y, nextPos.z);

                // We can only flow through Air or Water
                if (nType == AIR || nType == WATER_FLOWING || nType == WATER_SOURCE) {
                    visited.insert(nextPos);
                    queue.push_back({ nextPos, dist + 1 });
                }
            }
        }
    }

    // Return a high cost if no drop-off was found
    return 1000;
}

// 2. PHYSICS LOGIC WITH ANTI-STACKING RESTORED
void World::processWater(int x, int y, int z, uint8_t type) {

    // ==========================================================
    // 1. VALIDATION (The "Drying Up" Logic)
    // ==========================================================

    if (type == WATER_FLOWING) {
        int newStrength = 0;

        // A. Check Above (Waterfall)
        uint8_t typeAbove = getBlock(x, y + 1, z);
        if (typeAbove == WATER_SOURCE || typeAbove == WATER_FLOWING) {
            newStrength = MAX_FLUID_LEVEL;
        }
        else {
            // B. Check Horizontal Neighbors
            int maxNeighborStrength = 0;
            glm::ivec3 neighbors[] = { {1,0,0}, {-1,0,0}, {0,0,1}, {0,0,-1} };

            for (auto& n : neighbors) {
                int nx = x + n.x;
                int nz = z + n.z;
                uint8_t nType = getBlock(nx, y, nz);

                if (nType == WATER_SOURCE) {
                    maxNeighborStrength = MAX_FLUID_LEVEL;
                }
                else if (nType == WATER_FLOWING) {
                    int nStrength = getBlockMeta(nx, y, nz);
                    if (nStrength > maxNeighborStrength) {
                        maxNeighborStrength = nStrength;
                    }
                }
            }
            newStrength = maxNeighborStrength - 1;
        }

        // C. Apply Decay
        int currentStrength = getBlockMeta(x, y, z);

        if (newStrength < 0) newStrength = 0;

        if (newStrength != currentStrength) {
            if (newStrength <= 0) {
                setBlock(x, y, z, AIR);
            }
            else {
                setBlockMeta(x, y, z, newStrength);
            }

            return;
        }
    }

    // ==========================================================
    // 2. SPREADING LOGIC
    // ==========================================================

    int strength = (type == WATER_SOURCE) ? MAX_FLUID_LEVEL : getBlockMeta(x, y, z);

    if (strength <= 0) return;

    // --- Vertical Flow (Gravity) ---
    uint8_t blockBelow = getBlock(x, y - 1, z);
    uint8_t metaBelow = getBlockMeta(x, y - 1, z);

    bool isAirBelow = (blockBelow == AIR);
    bool isWaterBelow = (blockBelow == WATER_FLOWING || blockBelow == WATER_SOURCE);

    if (blockBelow != WATER_SOURCE && (isAirBelow || (isWaterBelow && metaBelow != MAX_FLUID_LEVEL))) {
        setBlock(x, y - 1, z, WATER_FLOWING);
        setBlockMeta(x, y - 1, z, MAX_FLUID_LEVEL);
        return;
    }

    // --- Anti-Stacking (Smarter Version) ---
    if (isWaterBelow && blockBelow != WATER_SOURCE) {
        if (strength < MAX_FLUID_LEVEL) {
            return;
        }
    }

    // --- Horizontal Flow ---
    if (strength <= 1) return;
    std::vector<glm::ivec3> flowDirs = getBestFlowDirections(x, y, z);
    for (auto& dir : flowDirs) {
        int nx = x + dir.x;
        int nz = z + dir.z;

        uint8_t nType = getBlock(nx, y, nz);
        int nStrength = getBlockMeta(nx, y, nz);

        if (BlockRegistry::Get().IsSolid(nType)) continue;
        if (nType == WATER_SOURCE) continue;

        // === NEW ENCLOSURE HEURISTIC ===
        // Check if the target location sits on top of existing water (Stacking).
        uint8_t targetBlockBelow = getBlock(nx, y - 1, nz);
        if (targetBlockBelow == WATER_SOURCE || targetBlockBelow == WATER_FLOWING) {

            // Count how many Solid or Liquid neighbors the TARGET has.
            int enclosure = 0;
            glm::ivec3 checkDirs[] = { {1,0,0}, {-1,0,0}, {0,0,1}, {0,0,-1} };

            for (auto& cd : checkDirs) {
                uint8_t nb = getBlock(nx + cd.x, y, nz + cd.z);
                if (BlockRegistry::Get().IsSolid(nb) || BlockRegistry::Get().IsLiquid(nb)) {
                    enclosure++;
                }
            }

            // If the target has fewer than 2 neighbors, it's "Open Air" (like a slope).
            // Don't stack there.
            // If it has 2+ neighbors, it's a "Hole" or "Channel". Fill it.
            if (enclosure < 2) continue;
        }
        // ===============================

        int spreadStrength = strength - 1;
        if (nType == AIR || (nType == WATER_FLOWING && nStrength < spreadStrength)) {
            setBlock(nx, y, nz, WATER_FLOWING);
            setBlockMeta(nx, y, nz, spreadStrength);
        }
    }
}

std::vector<glm::ivec3> World::getBestFlowDirections(int x, int y, int z) {
    std::vector<glm::ivec3> directions;
    int costs[4];
    
    int minCost = 99999;

    glm::ivec3 offsets[] = { {1,0,0}, {-1,0,0}, {0,0,1}, {0,0,-1} };

    for (int i = 0; i < 4; i++) {
        int nx = x + offsets[i].x;
        int nz = z + offsets[i].z;

        uint8_t nType = getBlock(nx, y, nz);

        if (nType != AIR && nType != WATER_FLOWING && nType != WATER_SOURCE) {
            costs[i] = 99999; 
        }
        else {
            costs[i] = calculateFlowCost(nx, y, nz, 1);
        }

        if (costs[i] < minCost) minCost = costs[i]; //what does this line do because if i comment it out the water will not spread...
    }

    // 3. Populate directions
    if (minCost < 99999) {
        for (int i = 0; i < 4; i++) {
            if (costs[i] == minCost) {
                directions.push_back(offsets[i]);
            }
        }
    }

    return directions;
}