#pragma once
#include <vector>
#include <memory>

#include "Mesh.h"
#include "CubeVertices.h"

class Chunk {
public:
    static const int CHUNK_SIZE = 16;

    Chunk(int x, int z);
    ~Chunk();

    void update();
    void draw(Shader& shader);

    bool isBlockSolid(int x, int y, int z);
private:
    uint8_t m_blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
    int chunkX;
    int chunkZ;

    std::unique_ptr<Mesh> m_mesh;

    void addFace(const std::array<Vertex, 6>& face, int x, int y, int z, std::vector<Vertex>& vertices);
};