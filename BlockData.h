#pragma once
#include <string>
#include <vector>
#include <cstdint>

enum BLOCK_TYPE : uint8_t
{
    AIR = 0,
    STONE,
    DIRT,
    GRASS,
    TNT,
    GLASS,
    WATER_SOURCE,
    WATER_FLOWING,
    NUM_TYPES,
};

struct TextureCoords {
    int x, y;
};

struct BlockDefinition {
    std::string name = "Unknown";

    bool isSolid = true;
    bool isTransparent = false;
    bool isLiquid = false;

    TextureCoords top = { 0, 0 };
    TextureCoords side = { 0, 0 };
    TextureCoords bottom = { 0, 0 };
};

class BlockRegistry {
public:
    static BlockRegistry& Get();

    void Init();

    const BlockDefinition& GetBlock(uint8_t id) const;

    bool IsSolid(uint8_t id) const;
    bool IsTransparent(uint8_t id) const;
    bool IsLiquid(uint8_t id) const;

private:
    BlockRegistry();
    std::vector<BlockDefinition> m_blocks;
};