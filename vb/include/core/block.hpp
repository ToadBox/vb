#ifndef VB_BLOCK_HPP
#define VB_BLOCK_HPP

#include <cstdint>
#include <string>

#include "core/registry_object.hpp"

namespace vb {

enum class BlockRenderType : uint32_t {
    OPAQUE,
    TRANSPARENT,
    LIQUID
};

class Block : public RegistryObject {
public:
    Block() : RegistryObject("Unnamed_Block") {};
    Block(const std::string& name, BlockRenderType render_type, float hardness) : 
        RegistryObject(name), render_type(render_type), hardness(hardness) {};
    Block(const vb::Block& other) : RegistryObject(other), render_type(other.render_type) {
        hardness = other.hardness;
    };
    Block(vb::Block&& other) : RegistryObject(std::move(other)), render_type(std::move(other.render_type)) {
        hardness = other.hardness;
    };

    Block& operator=(const Block& other) {
        if (this != &other) {
            RegistryObject::operator=(other);
            render_type = other.render_type;
            hardness = other.hardness;
        }
        return *this;
    }

    BlockRenderType GetRenderType() {return render_type;};
private:
    BlockRenderType render_type;
    float hardness;
};

}

#endif