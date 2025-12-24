#pragma once
#include <unordered_map>
#include <memory>
#include <cmath>
#include <deque>
#include <unordered_set>

#include "BlockData.h"
#include "Chunk.h"
#include "Shader.h"
#include "AABB.h"

struct BlockUpdate {
    int x, y, z;
    uint8_t type;
    int level;
};

struct RayHit {
    bool hit;
    glm::ivec3 position;
    glm::ivec3 normal;
};

struct ChunkCoord {
    int x, z;
    bool operator==(const ChunkCoord& other) const {
        return x == other.x && z == other.z;
    }
};


struct ChunkCoordHash {
    std::size_t operator()(const ChunkCoord& k) const {
        return std::hash<int>()(k.x) ^ (std::hash<int>()(k.z) << 1);
    }
};

struct Ivec3Hash {
    std::size_t operator()(const glm::ivec3& k) const {
        return std::hash<int>()(k.x) ^ (std::hash<int>()(k.y) << 1) ^ (std::hash<int>()(k.z) << 2);
    }
};

namespace std {
    template<> struct hash<glm::ivec3> {
        size_t operator()(const glm::ivec3& v) const {
            return hash<int>()(v.x) ^ (hash<int>()(v.y) << 1) ^ (hash<int>()(v.z) << 2);
        }
    };
}

class World
{
private:
    std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>, ChunkCoordHash> chunks;

    int chunksToLoadPerFrame = 4;

    std::deque<glm::ivec3> m_fluidQueue;
    std::unordered_set<glm::ivec3> m_pendingUpdates; 
    float m_fluidTimer = 0.0f;
    const float FLUID_TICK_RATE = 0.05f;

    void addFluidUpdate(int x, int y, int z);
    void processWater(int x, int y, int z, uint8_t type);
    int calculateFlowCost(int x, int y, int z, int recursionDepth);
    std::vector<glm::ivec3> getBestFlowDirections(int x, int y, int z);

    Chunk* getChunk(int x, int z);

public:
    const uint8_t MAX_FLUID_LEVEL = 6;

    World();
    void draw(Shader& shader);
    void update(glm::vec3 playerPosition);
    void updateFluids(float dt);

    bool checkCollision(const AABB& box);

    uint8_t getBlock(int x, int y, int z);
    void setBlock(int x, int y, int z, uint8_t type);

    void setBlockMeta(int x, int y, int z, uint8_t data);
    uint8_t getBlockMeta(int x, int y, int z);

    RayHit RayCast(glm::vec3 start, glm::vec3 dir, float maxDist);

    bool isBlockSolid(int x, int y, int z);
    int renderDistance = 16;
};