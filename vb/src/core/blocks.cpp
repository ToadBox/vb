#include "core/blocks.hpp"

#include "spdlog/spdlog.h"

namespace {
    vb::Registry<vb::Block> block_registry;
}

bool vb::Blocks::registerBlock(vb::Block&& block, uint64_t ID) {
        return block_registry.registerObject(std::move(block), ID);
};

bool vb::Blocks::registerBlock(Block&& block, ByID ID) {
    return registerBlock(std::move(block), (uint64_t) ID);
}

const vb::Block* vb::Blocks::getBlockByID(uint64_t ID) {
    return block_registry.getObject(ID);
};

void vb::Blocks::registerBlocks() { 
    vb::Blocks::registerBlock(vb::Block("air", vb::BlockRenderType::TRANSPARENT, 0.0f), vb::Blocks::ByID::AIR);
    vb::Blocks::registerBlock(vb::Block("dirt", vb::BlockRenderType::OPAQUE, 0.3f), vb::Blocks::ByID::DIRT);
    vb::Blocks::registerBlock(vb::Block("grass", vb::BlockRenderType::OPAQUE, 0.5f), vb::Blocks::ByID::GRASS);
    vb::Blocks::registerBlock(vb::Block("stone", vb::BlockRenderType::OPAQUE, 1.0f), vb::Blocks::ByID::STONE);
}