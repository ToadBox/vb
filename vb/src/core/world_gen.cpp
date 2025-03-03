#include "core/world_gen.hpp"

#include "core/chunk.hpp"

void vb::WorldGenerator::temperatureSeed(uint32_t seed) {
    temperature_seed = seed;
}

void vb::WorldGenerator::humiditySeed(uint32_t seed) {
    humidity_seed = seed;
}

void vb::WorldGenerator::worldSeed(uint32_t seed) {
    world_seed = seed;
}

void vb::WorldGenerator::generateChunk(ChunkData* const data) const {
    // place layer of grass at y=15
    for (uint8_t x = 0; x < Chunk::SIZE; x++) {
        for (uint8_t z = 0; z < Chunk::SIZE; z++) {
            auto pos = ChunkPos(x, Chunk::SIZE-1, z);
            data->set(pos, Blocks::ByID::GRASS);
        }
    }
    // spdlog::critical(data.blocks.size());
    // place 5 layers of dirt
    for (uint8_t x = 0; x < Chunk::SIZE; x++) {
        for (uint8_t z = 0; z < Chunk::SIZE; z++) {
            for (int8_t y = Chunk::SIZE-2; y > Chunk::SIZE-8; y--) {
                data->set(ChunkPos(x, y, z), Blocks::ByID::DIRT);
            }
        }
    }
    // place many layers of stone
    for (uint8_t x = 0; x < Chunk::SIZE; x++) {
        for (uint8_t z = 0; z < Chunk::SIZE; z++) {
            for (int8_t y = Chunk::SIZE-8; y >= 0; y--) {
                data->set(ChunkPos(x, y, z), Blocks::ByID::STONE);
            }
        }
    }
}