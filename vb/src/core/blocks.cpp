#include "core/blocks.hpp"

#include "spdlog/spdlog.h"

bool vb::Blocks::registerBlock(Block&& block, uint64_t ID) {
    if (block_registry.contains(ID)) {
        spdlog::warn("Block ID ({}) conflict detected", ID);
        return false;
    }
}