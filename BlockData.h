#pragma once
#include <cstdint>

enum BLOCK_TYPE : uint8_t
{
    AIR,
    STONE,
    DIRT,
    GRASS,
    TNT,
    GLASS,
};

struct BlockProps {
    int topX, topY;
    int sideX, sideY;
    int botX, botY;
};

static const BlockProps BLOCK_DATABASE[] = {
    {0, 0, 0, 0, 0, 0}, // AIR
    {1, 15, 1, 15, 1, 15}, // STONE
    {2, 15, 2, 15, 2, 15}, // DIRT
    {0, 15, 3, 15, 2, 15}, // GRASS
    {9, 15, 8, 15, 10, 15}, // TNT
    {1, 12, 1, 12, 1, 12} // GLASS
};

inline const BlockProps& GetBlockProps(uint8_t blockType) {
    return BLOCK_DATABASE[blockType];
}

inline bool isBlockTransparent(uint8_t blockType) {
    return blockType == AIR || blockType == GLASS;
}