#include "Chunk.h"
#include "BlockData.h"
#include "World.h"
#include <cmath>

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin/stb_perlin.h"

Chunk::Chunk(World* world, int x, int z)
    : m_world(world), chunkX(x), chunkZ(z)
{
    float frequency = 0.05f;
    int amplitude = 16;
    int baseHeight = 4;

    for (int lx = 0; lx < CHUNK_SIZE; lx++) {
        for (int lz = 0; lz < CHUNK_SIZE; lz++) {
            // global coords
            float globalX = (float)(chunkX * CHUNK_SIZE + lx);
            float globalZ = (float)(chunkZ * CHUNK_SIZE + lz);

            float noiseValue = stb_perlin_noise3(globalX * frequency, globalZ * frequency, 0.0f, 0, 0, 0);

            int height = baseHeight + (int)((noiseValue + 1.0f) * 0.5f * amplitude);
            for (int ly = 0; ly < CHUNK_SIZE; ly++) {
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
}

Chunk::~Chunk() {

}

bool Chunk::isBlockSolid(int x, int y, int z) {
    // 1. Check height bounds (assuming 1 chunk high world for now)
    if (y < 0 || y >= CHUNK_SIZE) {
        return false;
    }

    // 2. Check if inside this chunk
    if (x >= 0 && x < CHUNK_SIZE &&
        z >= 0 && z < CHUNK_SIZE) {
        return m_blocks[x][y][z] != 0;
    }

    // 3. If outside, ask the World for the neighbor block
    int globalX = chunkX * CHUNK_SIZE + x;
    int globalZ = chunkZ * CHUNK_SIZE + z;

    // Note: getBlock returns 0 (AIR) if the chunk doesn't exist yet
    return m_world->getBlock(globalX, y, globalZ) != 0;
}

uint8_t Chunk::getBlock(int x, int y, int z)
{
    return m_blocks[x][y][z];
}

void Chunk::setBlock(int x, int y, int z, uint8_t type)
{
    m_blocks[x][y][z] = type;
}

void Chunk::addFace(const std::array<Vertex, 6>& faceVertices, int x, int y, int z, int texX, int texY, std::vector<Vertex>& vertices) {

    // Calculate the UV width/height of a single tile
    // Example: 1.0 / 16 = 0.0625
    float tileWidth = 1.0f / (float)ATLAS_WIDTH;
    float tileHeight = 1.0f / (float)ATLAS_HEIGHT;

    for (int i = 0; i < 6; i++) {
        Vertex v = faceVertices[i];

        // Offset Position
        v.Position.x += x + 0.5f;
        v.Position.y += y + 0.5f;
        v.Position.z += z + 0.5f;

        v.TexCoords.x = (v.TexCoords.x + texX) * tileWidth;
        v.TexCoords.y = (v.TexCoords.y + texY) * tileHeight;

        vertices.push_back(v);
    }
}

void Chunk::update() {
    std::vector<Vertex> vertices;


    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {

                uint8_t type = m_blocks[x][y][z];

                if (type == AIR) continue;

                const BlockProps& props = GetBlockProps(type);

                // Left (-X)
                if (!isBlockSolid(x - 1, y, z))
                    addFace(VERTICES::LEFT_FACE, x, y, z, props.sideX, props.sideY, vertices);

                // Right (+X)
                if (!isBlockSolid(x + 1, y, z))
                    addFace(VERTICES::RIGHT_FACE, x, y, z, props.sideX, props.sideY, vertices);

                // Bottom (-Y)
                if (!isBlockSolid(x, y - 1, z))
                    addFace(VERTICES::BOTTOM_FACE, x, y, z, props.botX, props.botY, vertices);

                // Top (+Y)
                if (!isBlockSolid(x, y + 1, z))
                    addFace(VERTICES::TOP_FACE, x, y, z, props.topX, props.topY, vertices);

                // Front (+Z)
                if (!isBlockSolid(x, y, z - 1))
                    addFace(VERTICES::FRONT_FACE, x, y, z, props.sideX, props.sideY, vertices);
                // Back (-Z)
                if (!isBlockSolid(x, y, z + 1))
                    addFace(VERTICES::BACK_FACE, x, y, z, props.sideX, props.sideY, vertices);
            }
        }
    }

    m_mesh = std::make_unique<Mesh>(vertices);
}

void Chunk::draw(Shader& shader) {
    if (m_mesh)
    {
        float globalX = chunkX * CHUNK_SIZE;
        float globalZ = chunkZ * CHUNK_SIZE;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(globalX, 0.0f, globalZ));

        shader.setMat4("model", model);

        m_mesh->draw(shader);
    }
}