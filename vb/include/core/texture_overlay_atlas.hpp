#ifndef VB_TEXTURE_OVERLAY_ATLAS_HPP
#define VB_TEXTURE_OVERLAY_ATLAS_HPP

#include <cstdint>
#include <unordered_map>

#include "core/texture_region.hpp"
#include "core/block.hpp"

namespace vb {

class TextureOverlayAtlas {
public:
    static const uint8_t TEX_SIZE = 32;
    TextureOverlayAtlas(const TextureOverlayAtlas&) = delete;
    TextureOverlayAtlas& operator=(const TextureOverlayAtlas&) = delete;

    static TextureOverlayAtlas& getAtlas() {
        static TextureOverlayAtlas instance;
        return instance;
    }

    void bindTextureRegion(uint64_t ID, const TextureRegion& region);
    void bindTextureRegion(uint64_t ID, uint64_t x, uint64_t y, uint32_t length);

    void loadFromFile(const char* loc);
    
    NormalizedTextureRegion getNormalizedTexCoords(const uint64_t ID, Face face=Face::TOP);

    unsigned int getTex();
private:
    TextureOverlayAtlas();
    int width, height, nrChannels;
    unsigned int texture = 0;
    std::unordered_map<uint64_t, TextureRegion> atlas;
}; 



}

#endif