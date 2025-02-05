#include "core/world.hpp"

#include "spdlog/spdlog.h"

vb::World::World() {
    spdlog::info("World constructo");
}

vb::World::~World() {
    spdlog::info("World destructo");
}

void vb::World::update() {
    chunks.clear();
    chunks[ChunkPos(0, 0, 0)] = Chunk(ChunkPos(0, 0, 0));

    for (auto& [pos, chunk] : chunks) {
        chunk.updateChunk();
    }
}