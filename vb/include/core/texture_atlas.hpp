#ifndef VB_TEXTURE_ATLAS_HPP
#define VB_TEXTURE_ATLAS_HPP

#include <cstdint>
#include <unordered_map>

#include "core/texture_region.hpp"

namespace vb {
struct Blocks;

class TextureAtlas {
public:
    TextureAtlas(const TextureAtlas&) = delete;
    TextureAtlas& operator=(const TextureAtlas&) = delete;

    static TextureAtlas& getAtlas() {
        static TextureAtlas instance;
        return instance;
    }

    void bindTextureRegion(const uint64_t ID, const TextureRegion& region);
    void bindTextureRegion(const uint64_t ID, uint64_t x, uint64_t y, uint64_t width=32, uint64_t height=32);

    void loadFromFile(const char* loc);
    
    NormalizedTextureRegion getNormalizedTexCoords(const uint64_t ID);

    unsigned int getTex();
private:
    TextureAtlas();
    int width, height, nrChannels;
    unsigned int texture = 0;
    std::unordered_map<uint64_t, TextureRegion> atlas;
}; 



}

#endif