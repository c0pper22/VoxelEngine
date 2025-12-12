#include "Chunk.h"

Chunk::Chunk(int x, int z)
    : chunkX(x), chunkZ(z)
{
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                m_blocks[x][y][z] = 1;
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

void Chunk::addFace(const std::array<Vertex, 6>& faceVertices, int x, int y, int z, std::vector<Vertex>& vertices) {
    for (int i = 0; i < 6; i++) {
        Vertex v = faceVertices[i];
        v.Position.x += x;
        v.Position.y += y;
        v.Position.z += z;
        vertices.push_back(v);
    }
}

void Chunk::update() {
    std::vector<Vertex> vertices;
    std::vector<Texture> textures;

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {

                if (!isBlockSolid(x, y, z)) continue;

                // Left (-X)
                if (!isBlockSolid(x - 1, y, z))
                    addFace(VERTICES::LEFT_FACE, x, y, z, vertices);

                // Right (+X)
                if (!isBlockSolid(x + 1, y, z))
                    addFace(VERTICES::RIGHT_FACE, x, y, z, vertices);

                // Bottom (-Y)
                if (!isBlockSolid(x, y - 1, z))
                    addFace(VERTICES::BOTTOM_FACE, x, y, z, vertices);

                // Top (+Y)
                if (!isBlockSolid(x, y + 1, z))
                    addFace(VERTICES::TOP_FACE, x, y, z, vertices);

                // Back (+Z)
                if (!isBlockSolid(x, y, z - 1))
                    addFace(VERTICES::FRONT_FACE, x, y, z, vertices);

                // Back (-Z)
                if (!isBlockSolid(x, y, z + 1))
                    addFace(VERTICES::BACK_FACE, x, y, z, vertices);
            }
        }
    }

    m_mesh = std::make_unique<Mesh>(vertices, textures);
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