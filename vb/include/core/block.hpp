#ifndef VB_BLOCK_HPP
#define VB_BLOCK_HPP

#include <cstdint>
#include <string>

namespace vb {

enum class BlockRenderType : uint32_t {
    SOLID,
    TRANSPARENT,
    LIQUID
};

class Block {
public:
    Block(const std::string& name);

private:
    std::string name;
    BlockRenderType render_type;
};

}

#endif