#pragma once
#include <vector>
#include <memory>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Mesh.h"
#include "CubeVertices.h"
#include "Texture.h"

class World;

class Chunk {
public:
    static const int CHUNK_SIZE = 16;
    static const int CHUNK_HEIGHT = 128;
    static const int ATLAS_WIDTH = 16;
    static const int ATLAS_HEIGHT = 16;

    int chunkX;
    int chunkZ;

    Chunk(World* world, int x, int z);
    ~Chunk();

    void update();
    void drawOpaque(Shader& shader);
    void drawTransparent(Shader& shader);

    bool isBlockSolid(int x, int y, int z);
    uint8_t getBlock(int x, int y, int z);
    void setBlock(int x, int y, int z, uint8_t type);
private:
    uint8_t m_blocks[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];

    World* m_world;

    std::unique_ptr<Mesh> m_mesh;
    std::unique_ptr<Mesh> m_transparentMesh;

    void addFace(const std::array<Vertex, 6>& face, int x, int y, int z, int texX, int texY, std::vector<Vertex>& vertices);
};