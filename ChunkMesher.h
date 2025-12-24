#pragma once
#include <vector>
#include <array>
#include "Mesh.h"
#include "Chunk.h"

struct ChunkMeshData {
    std::vector<Vertex> opaqueVertices;
    std::vector<Vertex> transparentVertices;
};

class ChunkMesher
{
public:
    static std::vector<Vertex> s_opaqueVertices;
    static std::vector<Vertex> s_transparentVertices;

    static ChunkMeshData GenerateMesh(Chunk& chunk);

private:
    static bool shouldRenderFace(const Chunk& chunk, int x, int y, int z, uint8_t currentBlockType, uint8_t currentMeta);

    static void addFace(const std::array<Vertex, 6>& faceVertices, int x, int y, int z, int texX, int texY, std::vector<Vertex>& vertices, float height, bool isSideFace);
};