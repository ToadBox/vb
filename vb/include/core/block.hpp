#ifndef VB_BLOCK_HPP
#define VB_BLOCK_HPP

#include <cstdint>
#include <string>

#include "core/registry_object.hpp"

namespace vb {

enum class BlockRenderType : uint32_t {
    SOLID,
    TRANSPARENT,
    LIQUID
};

class Block : RegistryObject {
public:
    Block(const std::string& name);

    BlockRenderType GetRenderType() {return render_type;};
private:
    BlockRenderType render_type;
    float hardness;
};

}

#endif