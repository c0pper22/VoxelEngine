#include "ChunkMesher.h"
#include "BlockData.h"
#include "World.h"
#include "CubeVertices.h"

ChunkMeshData ChunkMesher::GenerateMesh(Chunk& chunk)
{
    ChunkMeshData data;

    for (int x = 0; x < Chunk::CHUNK_SIZE; x++) {
        for (int y = 0; y < Chunk::CHUNK_HEIGHT; y++) {
            for (int z = 0; z < Chunk::CHUNK_SIZE; z++) {

                uint8_t type = chunk.getBlock(x, y, z);
                if (type == AIR) continue;

                const BlockDefinition& def = BlockRegistry::Get().GetBlock(type);
                bool isTransparent = def.isTransparent;

                std::vector<Vertex>* targetList = isTransparent ?
                    &data.transparentVertices : &data.opaqueVertices;

                float h = 1.0f;
                uint8_t currentMeta = chunk.getMeta(x, y, z);

                bool sameLiquidAbove = false;

                if (def.isLiquid) {
                    if (type == WATER_SOURCE) {
                        h = 1.0f;
                    }
                    else {
                        h = (float)(currentMeta + 1) / 12.0f;
                        if (h > 1.0f) h = 1.0f;
                    }

                    if (y < Chunk::CHUNK_HEIGHT - 1) {
                        uint8_t blockAbove = chunk.getBlock(x, y + 1, z);

                        if (blockAbove == type) {
                            sameLiquidAbove = true;
                        }

                        if (BlockRegistry::Get().IsLiquid(blockAbove)) {
                            h = 1.0f;
                        }
                    }
                }

                // === GENERATE FACES ===
                if (shouldRenderFace(chunk, x - 1, y, z, type, currentMeta))
                    addFace(VERTICES::LEFT_FACE, x, y, z, def.side.x, def.side.y, *targetList, h, true);

                if (shouldRenderFace(chunk, x + 1, y, z, type, currentMeta))
                    addFace(VERTICES::RIGHT_FACE, x, y, z, def.side.x, def.side.y, *targetList, h, true);

                if (shouldRenderFace(chunk, x, y - 1, z, type, currentMeta))
                    addFace(VERTICES::BOTTOM_FACE, x, y, z, def.bottom.x, def.bottom.y, *targetList, h, false);

                if (shouldRenderFace(chunk, x, y + 1, z, type, currentMeta)) {
                    if (!def.isLiquid || !sameLiquidAbove) {
                        addFace(VERTICES::TOP_FACE, x, y, z, def.top.x, def.top.y, *targetList, h, false);
                    }
                }
                if (shouldRenderFace(chunk, x, y, z - 1, type, currentMeta))
                    addFace(VERTICES::FRONT_FACE, x, y, z, def.side.x, def.side.y, *targetList, h, true);

                if (shouldRenderFace(chunk, x, y, z + 1, type, currentMeta))
                    addFace(VERTICES::BACK_FACE, x, y, z, def.side.x, def.side.y, *targetList, h, true);
            }
        }
    }

    return data;
}

// CHANGED: Updated signature and added Liquid Logic
bool ChunkMesher::shouldRenderFace(const Chunk& chunk, int neighborX, int neighborY, int neighborZ, uint8_t currentBlockType, uint8_t currentMeta)
{
    uint8_t neighborType = 0;
    uint8_t neighborMeta = 0;

    // 1. Get Neighbor Type AND Meta safely (checking chunk bounds)
    if (neighborX >= 0 && neighborX < Chunk::CHUNK_SIZE &&
        neighborY >= 0 && neighborY < Chunk::CHUNK_HEIGHT &&
        neighborZ >= 0 && neighborZ < Chunk::CHUNK_SIZE) {
        neighborType = chunk.getBlock(neighborX, neighborY, neighborZ);
        neighborMeta = chunk.getMeta(neighborX, neighborY, neighborZ);
    }
    else {
        // Neighbor is in another chunk - Access via World
        int globalX = chunk.chunkX * Chunk::CHUNK_SIZE + neighborX;
        int globalZ = chunk.chunkZ * Chunk::CHUNK_SIZE + neighborZ;
        // Note: You might need to make 'm_world' public in Chunk.h or add a getter, 
        // but based on your Chunk.cpp, 'm_world' is accessible here.
        neighborType = chunk.m_world->getBlock(globalX, neighborY, globalZ);
        neighborMeta = chunk.m_world->getBlockMeta(globalX, neighborY, globalZ);
    }

    bool isCurrentTransparent = BlockRegistry::Get().IsTransparent(currentBlockType);
    bool isNeighborTransparent = BlockRegistry::Get().IsTransparent(neighborType);

    if (isCurrentTransparent) {
        if (neighborType == currentBlockType && BlockRegistry::Get().IsLiquid(currentBlockType)) {
            return currentMeta > neighborMeta;
        }

        return isNeighborTransparent && (neighborType != currentBlockType);
    }

    return isNeighborTransparent;
}

void ChunkMesher::addFace(const std::array<Vertex, 6>& faceVertices, int x, int y, int z, int texX, int texY, std::vector<Vertex>& vertices, float height, bool isSideFace)
{
    float tileWidth = 1.0f / (float)Chunk::ATLAS_WIDTH;
    float tileHeight = 1.0f / (float)Chunk::ATLAS_HEIGHT;

    for (int i = 0; i < 6; i++) {
        Vertex v = faceVertices[i];

        if (v.Position.y > 0.0f) {
            v.Position.y = -0.5f + height;

            if (isSideFace) {
                v.TexCoords.y *= height;
            }
        }

        // Offset Position
        v.Position.x += x + 0.5f;
        v.Position.y += y + 0.5f;
        v.Position.z += z + 0.5f;

        // Texture Atlas Calculation
        v.TexCoords.x = (v.TexCoords.x + texX) * tileWidth;
        v.TexCoords.y = (v.TexCoords.y + texY) * tileHeight;

        vertices.push_back(v);
    }
}