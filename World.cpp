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

void World::update(glm::vec3 playerPosition)
{
    int playerChunkX = static_cast<int>(std::floor(playerPosition.x / Chunk::CHUNK_SIZE));
    int playerChunkZ = static_cast<int>(std::floor(playerPosition.z / Chunk::CHUNK_SIZE));

    // --- STEP 1: Unload chunks ---
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

    // --- STEP 2: Load new chunks ---
    int chunksLoaded = 0;
    for (int x = playerChunkX - renderDistance; x <= playerChunkX + renderDistance; x++) {
        for (int z = playerChunkZ - renderDistance; z <= playerChunkZ + renderDistance; z++) {

            if (chunksLoaded >= chunksToLoadPerFrame) return;

            ChunkCoord coord = { x, z };
            if (chunks.find(coord) == chunks.end()) {
                auto newChunk = std::make_unique<Chunk>(this, x, z);
                newChunk->update();
                chunks[coord] = std::move(newChunk);
                chunksLoaded++;

                int worldX = x * Chunk::CHUNK_SIZE;
                int worldZ = z * Chunk::CHUNK_SIZE;
                if (Chunk* n = getChunk(worldX - Chunk::CHUNK_SIZE, worldZ)) n->update();
                if (Chunk* n = getChunk(worldX + Chunk::CHUNK_SIZE, worldZ)) n->update();
                if (Chunk* n = getChunk(worldX, worldZ - Chunk::CHUNK_SIZE)) n->update();
                if (Chunk* n = getChunk(worldX, worldZ + Chunk::CHUNK_SIZE)) n->update();
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
    chunk->update();

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
    if (localX == 0) { Chunk* n = getChunk(x - 1, z); if (n) n->update(); }
    if (localX == Chunk::CHUNK_SIZE - 1) { Chunk* n = getChunk(x + 1, z); if (n) n->update(); }
    if (localZ == 0) { Chunk* n = getChunk(x, z - 1); if (n) n->update(); }
    if (localZ == Chunk::CHUNK_SIZE - 1) { Chunk* n = getChunk(x, z + 1); if (n) n->update(); }
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

    addFluidUpdate(x, y, z);
    addFluidUpdate(x + 1, y, z);
    addFluidUpdate(x - 1, y, z);
    addFluidUpdate(x, y + 1, z);
    addFluidUpdate(x, y - 1, z);
    addFluidUpdate(x, y, z + 1);
    addFluidUpdate(x, y, z - 1);

    if (localX == 0) { Chunk* n = getChunk(x - 1, z); if (n) n->update(); }
    if (localX == Chunk::CHUNK_SIZE - 1) { Chunk* n = getChunk(x + 1, z); if (n) n->update(); }
    if (localZ == 0) { Chunk* n = getChunk(x, z - 1); if (n) n->update(); }
    if (localZ == Chunk::CHUNK_SIZE - 1) { Chunk* n = getChunk(x, z + 1); if (n) n->update(); }
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

        if (getBlock(mapPos.x, mapPos.y, mapPos.z) != 0) return { true, mapPos, rayNormal };
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

int World::calculateFlowCost(int x, int y, int z, int depth) {
    if (depth > 4) return 1000;

    uint8_t blockBelow = getBlock(x, y - 1, z);

    // --- CHECK FOR DROP-OFF ---
    // A drop-off is Air, or Water that is falling (unsupported).
    if (blockBelow == AIR) return 0;

    if (blockBelow == WATER_FLOWING || blockBelow == WATER_SOURCE) {
        uint8_t blockTwoBelow = getBlock(x, y - 2, z);
        // If the water below has no floor, it's a waterfall. Flow towards it.
        if (!BlockRegistry::Get().IsSolid(blockTwoBelow)) {
            return 0;
        }
    }

    // --- STANDARD SEARCH ---
    // We are on solid ground. Look for neighbors leading to a cliff.
    int minCost = 1000;

    glm::ivec3 neighbors[] = { {1,0,0}, {-1,0,0}, {0,0,1}, {0,0,-1} };
    for (auto& n : neighbors) {
        int nx = x + n.x;
        int nz = z + n.z;

        uint8_t nType = getBlock(nx, y, nz);

        // Pass through Air or Water
        if (nType == AIR || nType == WATER_FLOWING || nType == WATER_SOURCE) {
            int cost = calculateFlowCost(nx, y, nz, depth + 1);
            if (cost < minCost) minCost = cost;
        }
    }

    return minCost + 1;
}

// 2. PHYSICS LOGIC WITH ANTI-STACKING RESTORED
void World::processWater(int x, int y, int z, uint8_t type) {

    // ==========================================================
    // 1. VALIDATION (The "Drying Up" Logic)
    // ==========================================================
    // If this is FLOWING water, we must ensure it is still supported 
    // by a neighbor. If the Source was broken, this chain breaks here.

    if (type == WATER_FLOWING) {
        int newStrength = 0;

        // A. Check Above (Waterfall)
        // If water is falling directly on us, we stay at max level.
        uint8_t typeAbove = getBlock(x, y + 1, z);
        if (typeAbove == WATER_SOURCE || typeAbove == WATER_FLOWING) {
            newStrength = MAX_FLUID_LEVEL;
        }
        else {
            // B. Check Horizontal Neighbors
            // Find the highest neighbor level and subtract 1.
            int maxNeighborStrength = 0;
            glm::ivec3 neighbors[] = { {1,0,0}, {-1,0,0}, {0,0,1}, {0,0,-1} };

            for (auto& n : neighbors) {
                int nx = x + n.x;
                int nz = z + n.z;
                uint8_t nType = getBlock(nx, y, nz);

                if (nType == WATER_SOURCE) {
                    // Connected directly to a true source
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
        // If our calculated strength doesn't match our current reality, update and abort.
        int currentStrength = getBlockMeta(x, y, z);

        if (newStrength < 0) newStrength = 0;

        if (newStrength != currentStrength) {
            if (newStrength <= 0) {
                setBlock(x, y, z, AIR); // No support? Dry up.
            }
            else {
                setBlockMeta(x, y, z, newStrength); // Less support? Lower level.
            }
            // Important: If we changed state, we stop here. 
            // The update we just triggered on neighbors will handle the rest next tick.
            return;
        }
    }

    // ==========================================================
    // 2. SPREADING LOGIC
    // ==========================================================
    // If we survived the check above, try to spread to neighbors.

    int strength = (type == WATER_SOURCE) ? MAX_FLUID_LEVEL : getBlockMeta(x, y, z);

    if (strength <= 0) return;

    // --- Vertical Flow (Gravity) ---
    uint8_t blockBelow = getBlock(x, y - 1, z);
    uint8_t metaBelow = getBlockMeta(x, y - 1, z);

    bool isAirBelow = (blockBelow == AIR);
    bool isWaterBelow = (blockBelow == WATER_FLOWING || blockBelow == WATER_SOURCE);

    // If air below, or weaker water below, we fall.
    if (isAirBelow || (isWaterBelow && metaBelow != MAX_FLUID_LEVEL)) {
        setBlock(x, y - 1, z, WATER_FLOWING);
        setBlockMeta(x, y - 1, z, MAX_FLUID_LEVEL); // Reset strength when falling
        return; // Gravity prevents horizontal spread
    }

    // --- Anti-Stacking ---
    // If we are on top of water, don't spread horizontally 
    // (This prevents layers of oceans).
    if (isWaterBelow && blockBelow != WATER_SOURCE) {
        return;
    }

    // *** REMOVED: "Source Creation" Logic ***
    // (This is what you wanted removed. Water will no longer form new sources.)

    // --- Horizontal Flow ---
    if (strength <= 1) return; // Too weak to spread further

    std::vector<glm::ivec3> flowDirs = getBestFlowDirections(x, y, z);
    for (auto& dir : flowDirs) {
        int nx = x + dir.x;
        int nz = z + dir.z;

        uint8_t nType = getBlock(nx, y, nz);
        int nStrength = getBlockMeta(nx, y, nz);

        if (BlockRegistry::Get().IsSolid(nType)) continue;
        if (nType == WATER_SOURCE) continue;

        int spreadStrength = strength - 1;

        // Spread to Air, or update a neighbor if we are stronger than it
        if (nType == AIR || (nType == WATER_FLOWING && nStrength < spreadStrength)) {
            setBlock(nx, y, nz, WATER_FLOWING);
            setBlockMeta(nx, y, nz, spreadStrength);
        }
    }
}

std::vector<glm::ivec3> World::getBestFlowDirections(int x, int y, int z) {
    std::vector<glm::ivec3> directions;
    int costs[4];
    int minCost = 1000;

    glm::ivec3 offsets[] = { {1,0,0}, {-1,0,0}, {0,0,1}, {0,0,-1} };

    // Calculate cost for each direction
    for (int i = 0; i < 4; i++) {
        int nx = x + offsets[i].x;
        int nz = z + offsets[i].z;

        uint8_t nType = getBlock(nx, y, nz);

        // Blocked by solid block? Cost is high.
        if (nType != AIR && nType != WATER_FLOWING && nType != WATER_SOURCE) {
            costs[i] = 1000;
        }
        else {
            costs[i] = calculateFlowCost(nx, y, nz, 1);
        }

        if (costs[i] < minCost) minCost = costs[i];
    }

    // Add all directions that share the minimum cost
    for (int i = 0; i < 4; i++) {
        if (costs[i] == minCost) {
            directions.push_back(offsets[i]);
        }
    }
    return directions;
}