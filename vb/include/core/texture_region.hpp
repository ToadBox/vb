#ifndef VB_TEXTURE_REGION_HPP
#define VB_TEXTURE_REGION_HPP

#include <glm/vec4.hpp>

namespace vb {

struct TextureRegion {
    uint64_t x, y;
    uint32_t length;
};

struct NormalizedTextureRegion {
    float x0,y0, x1,y1;
};

}

#endif