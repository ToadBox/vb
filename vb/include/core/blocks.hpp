#ifndef BLOCKS_HPP
#define BLOCKS_HPP

#include <cstdint>
#include <unordered_map>

#include "core/registry.hpp"
#include "core/block.hpp"

namespace vb {

namespace Blocks {
    enum class ByID : uint64_t {
        AIR         = 0,
        DIRT        = 1,
        GRASS       = 2,
        STONE       = 3
    };

    bool registerBlock(Block&& block, uint64_t ID);
    bool registerBlock(Block&& block, ByID ID);
    
    const Block* getBlockByID(uint64_t ID);

    void registerBlocks();
}

}

#endif