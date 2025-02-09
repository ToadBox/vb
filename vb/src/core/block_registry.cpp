#include "core/block_registry.hpp"

#include "core/texture_atlas.hpp"

#include "spdlog/spdlog.h"

bool vb::Blocks::registerBlock(vb::Block&& block, uint64_t ID, const TextureRegion& region) {
    vb::TextureAtlas::getAtlas().bindTextureRegion(ID, region);
    return block_registry.registerObject(std::move(block), ID);
};

bool vb::Blocks::registerBlock(Block&& block, ByID ID, const TextureRegion& region) {
    return registerBlock(std::move(block), (uint64_t) ID, region);
}

const vb::Block* vb::Blocks::getBlockByID(uint64_t ID) {
    return block_registry.getObject(ID);
};

void vb::Blocks::registerBlocks() {
    vb::Blocks::registerBlock(vb::Block("air", vb::BlockRenderType::TRANSPARENT, 0.0f), vb::Blocks::ByID::AIR, {});
    vb::Blocks::registerBlock(vb::Block("dirt", vb::BlockRenderType::OPAQUE, 0.3f), vb::Blocks::ByID::DIRT, TextureRegion(32,0,31,31));
    vb::Blocks::registerBlock(vb::Block("grass", vb::BlockRenderType::OPAQUE, 0.5f), vb::Blocks::ByID::GRASS, TextureRegion(0,0,64,64));
    vb::Blocks::registerBlock(vb::Block("stone", vb::BlockRenderType::OPAQUE, 1.0f), vb::Blocks::ByID::STONE, TextureRegion(0,32,31,31));
}