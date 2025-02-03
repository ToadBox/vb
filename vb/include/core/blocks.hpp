#ifndef BLOCKS_HPP
#define BLOCKS_HPP

#include <cstdint>
#include <unordered_map>

#include "core/registry.hpp"
#include "core/block.hpp"

namespace {
    Registry<Block> block_registry;
    std::unordered_map<uint64_t, vb::Block> block_registry;
}

namespace vb {

namespace Blocks {
    bool registerBlock(Block&& block, uint64_t ID);
    Block& getBlockByID(uint64_t ID);
}

}

#endif