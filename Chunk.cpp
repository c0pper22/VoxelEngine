#include "Chunk.h"
#include "BlockData.h"

Chunk::Chunk(int x, int z)
    : chunkX(x), chunkZ(z)
{
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                m_blocks[x][y][z] = TNT;
            }
        }
    }

    update();
}

Chunk::~Chunk() {
}

bool Chunk::isBlockSolid(int x, int y, int z) {
    if (x < 0 || x >= CHUNK_SIZE ||
        y < 0 || y >= CHUNK_SIZE ||
        z < 0 || z >= CHUNK_SIZE) {
        return false;
    }
    return m_blocks[x][y][z] != 0;
}

void Chunk::addFace(const std::array<Vertex, 6>& faceVertices, int x, int y, int z, int texX, int texY, std::vector<Vertex>& vertices) {

    // Calculate the UV width/height of a single tile
    // Example: 1.0 / 16 = 0.0625
    float tileWidth = 1.0f / (float)ATLAS_WIDTH;
    float tileHeight = 1.0f / (float)ATLAS_HEIGHT;

    for (int i = 0; i < 6; i++) {
        Vertex v = faceVertices[i];

        // Offset Position
        v.Position.x += x;
        v.Position.y += y;
        v.Position.z += z;

        // Transform UVs for the Atlas
        // Formula: (originalUV + tileCoord) * tileSize
        // Note: originalUV is either 0.0 or 1.0 from CubeVertices.h

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