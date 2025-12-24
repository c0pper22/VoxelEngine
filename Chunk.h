#pragma once
#include <vector>
#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Mesh.h"
#include "Texture.h"

class World;

class Chunk {
    friend class ChunkMesher;
public:
    static const int CHUNK_SIZE = 16;
    static const int CHUNK_HEIGHT = 128;
    static const int ATLAS_WIDTH = 16;
    static const int ATLAS_HEIGHT = 16;

    std::atomic<bool> isMeshPending{ false };

    int chunkX;
    int chunkZ;

    Chunk(World* world, int x, int z);
    ~Chunk();

    void uploadMesh(const std::vector<Vertex>& opaque, const std::vector<Vertex>& transparent);

    void drawOpaque(Shader& shader);
    void drawTransparent(Shader& shader);

    bool isBlockSolid(int x, int y, int z);
    uint8_t getBlock(int x, int y, int z) const;
    void setBlock(int x, int y, int z, uint8_t type);

    uint8_t getMeta(int x, int y, int z) const;
    void setMeta(int x, int y, int z, uint8_t data);

private:
    uint8_t m_blocks[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];
    uint8_t m_metadata[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];

    World* m_world;
    std::unique_ptr<Mesh> m_mesh;
    std::unique_ptr<Mesh> m_transparentMesh;
};