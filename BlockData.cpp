#include "BlockData.h"

BlockRegistry& BlockRegistry::Get() {
    static BlockRegistry instance;
    return instance;
}

BlockRegistry::BlockRegistry() {
    // Reserve space to prevent reallocations
    m_blocks.resize(NUM_TYPES);
}

void BlockRegistry::Init() {
    // --- 0. AIR ---
    m_blocks[AIR] = { "Air", false, true, false };

    // --- 1. STONE ---
    m_blocks[STONE] = { "Stone", true, false, false,
        {1, 15}, {1, 15}, {1, 15} };

    // --- 2. DIRT ---
    m_blocks[DIRT] = { "Dirt", true, false, false,
        {2, 15}, {2, 15}, {2, 15} };

    // --- 3. GRASS ---
    m_blocks[GRASS] = { "Grass", true, false, false,
        {0, 15}, {3, 15}, {2, 15} };

    // --- 4. TNT ---
    m_blocks[TNT] = { "TNT", true, false, false,
        {9, 15}, {8, 15}, {10, 15} };

    // --- 5. GLASS ---
    m_blocks[GLASS] = { "Glass", true, true, false,
        {1, 12}, {1, 12}, {1, 12} };

    // --- 6. WATER SOURCE ---
    m_blocks[WATER_SOURCE] = { "Water Source", false, true, true,
        {13, 3}, {13, 3}, {13, 3} };

    // --- 7. WATER FLOWING ---
    m_blocks[WATER_FLOWING] = { "Water Flowing", false, true, true,
        {13, 3}, {13, 3}, {13, 3} };
}

const BlockDefinition& BlockRegistry::GetBlock(uint8_t id) const {
    if (id >= m_blocks.size()) return m_blocks[AIR];
    return m_blocks[id];
}

bool BlockRegistry::IsSolid(uint8_t id) const {
    return GetBlock(id).isSolid;
}

bool BlockRegistry::IsTransparent(uint8_t id) const {
    return GetBlock(id).isTransparent;
}

bool BlockRegistry::IsLiquid(uint8_t id) const {
    return GetBlock(id).isLiquid;
}