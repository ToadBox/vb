#ifndef BLOCKS_HPP
#define BLOCKS_HPP

#include <cstdint>
#include <unordered_map>

#include "core/registry.hpp"
#include "core/block.hpp"
#include "core/texture_region.hpp"

namespace vb {

class Blocks {
public:
    enum class ByID : uint64_t {
        AIR         = 0,
        DIRT        = 1,
        GRASS       = 2,
        STONE       = 3,
        SAND        = 4,
        GRAVEL      = 5,
        WATER       = 6
    };

    static Blocks& get() {
        static Blocks instance;
        return instance;
    }

    bool registerBlock(Block&& block, uint64_t ID, const TextureRegion& region);
    bool registerBlock(Block&& block, ByID ID, const TextureRegion& region);
    
    const Block* getBlockByID(uint64_t ID);

    void registerBlocks();

private:
    Blocks() = default;

    vb::Registry<vb::Block> block_registry;
};

}

#endif