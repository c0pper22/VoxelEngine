#include "BlockData.h"
#include "World.h"
#include "ChunkMesher.h" 
#include <cmath>

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin/stb_perlin.h"

Chunk::Chunk(World* world, int x, int z)
    : m_world(world), chunkX(x), chunkZ(z)
{
    float frequency = 0.05f;
    int amplitude = 24;
    int baseHeight = 4;

    for (int lx = 0; lx < CHUNK_SIZE; lx++) {
        for (int lz = 0; lz < CHUNK_SIZE; lz++) {
            // global coords
            float globalX = (float)(chunkX * CHUNK_SIZE + lx);
            float globalZ = (float)(chunkZ * CHUNK_SIZE + lz);

            float noiseValue = stb_perlin_noise3(globalX * frequency, globalZ * frequency, 0.0f, 0, 0, 0);
            int height = baseHeight + (int)((noiseValue + 1.0f) * 0.5f * amplitude);
            for (int ly = 0; ly < CHUNK_HEIGHT; ly++) {
                if (ly < height - 3) {
                    m_blocks[lx][ly][lz] = STONE;
                }
                else if (ly < height) {
                    m_blocks[lx][ly][lz] = DIRT;
                }
                else if (ly == height) {
                    m_blocks[lx][ly][lz] = GRASS;
                }
                else {
                    m_blocks[lx][ly][lz] = AIR;
                }
            }
        }
    }

    std::memset(m_metadata, 0, sizeof(m_metadata));
}

Chunk::~Chunk() {

}

void Chunk::uploadMesh(const std::vector<Vertex>& opaque, const std::vector<Vertex>& transparent) {
    m_mesh = std::make_unique<Mesh>(opaque);
    m_transparentMesh = std::make_unique<Mesh>(transparent);
    isMeshPending = false;
}

bool Chunk::isBlockSolid(int x, int y, int z) {
    if (y < 0 || y >= CHUNK_SIZE) return false;

    if (x >= 0 && x < CHUNK_SIZE &&
        z >= 0 && z < CHUNK_SIZE) {
        return m_blocks[x][y][z] != 0;
    }

    int globalX = chunkX * CHUNK_SIZE + x;
    int globalZ = chunkZ * CHUNK_SIZE + z;
    return m_world->getBlock(globalX, y, globalZ) != 0;
}

uint8_t Chunk::getBlock(int x, int y, int z) const
{
    return m_blocks[x][y][z];
}

void Chunk::setBlock(int x, int y, int z, uint8_t type)
{
    m_blocks[x][y][z] = type;
    m_metadata[x][y][z] = 0;
}

uint8_t Chunk::getMeta(int x, int y, int z) const {
    return m_metadata[x][y][z];
}

void Chunk::setMeta(int x, int y, int z, uint8_t data) {
    m_metadata[x][y][z] = data;
}

void Chunk::drawOpaque(Shader& shader) {
    if (!m_mesh) return;

    float globalX = (float)(chunkX * CHUNK_SIZE);
    float globalZ = (float)(chunkZ * CHUNK_SIZE);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(globalX, 0.0f, globalZ));
    shader.setMat4("model", model);

    m_mesh->draw(shader);
}

void Chunk::drawTransparent(Shader& shader) {
    if (!m_transparentMesh) return;

    float globalX = (float)(chunkX * CHUNK_SIZE);
    float globalZ = (float)(chunkZ * CHUNK_SIZE);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(globalX, 0.0f, globalZ));
    shader.setMat4("model", model);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDepthMask(GL_FALSE);

    m_transparentMesh->draw(shader);

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}