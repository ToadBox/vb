#include "core/texture_atlas.hpp"
#include "stb_image.h"
#include "glad/gl.h"
#include "spdlog/spdlog.h"

vb::TextureAtlas::TextureAtlas() {
    
}

void vb::TextureAtlas::bindTextureRegion(uint64_t ID, const TextureRegion& region) {
    atlas[ID] = region;
}

void vb::TextureAtlas::bindTextureRegion(uint64_t ID, uint64_t x, uint64_t y, uint32_t length) {
    atlas[ID] = {x,y,length};
}

void vb::TextureAtlas::loadFromFile(const char* loc) {
    if (texture) {
        glDeleteTextures(1, &texture);
    }
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    unsigned char *data = stbi_load(loc, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        spdlog::error("Failed to load texture atlas {}", loc);
    }
    stbi_image_free(data);
    // spdlog::info("{}x{} @depth of {}", width, height, nrChannels);
}

vb::NormalizedTextureRegion vb::TextureAtlas::getNormalizedTexCoords(const uint64_t ID, Face face) {
    TextureRegion region = atlas[ID];
    float width, height, x0, y0, x0_offset;

    switch (region.length) {
        case 3:
            switch (face) {
                case Face::NORTH:
                case Face::SOUTH:
                case Face::EAST:
                case Face::WEST:
                    x0_offset = TextureAtlas::TEX_SIZE;
                    break;
                case Face::BOT:
                    x0_offset = 2 * TextureAtlas::TEX_SIZE;
                    break;
                case Face::TOP:
                    x0_offset = 0;
                    break;
            }
            break;

        case 6:
            x0_offset = (uint8_t) face * TextureAtlas::TEX_SIZE;
            break;
        
        case 1:
        default:
            x0_offset = 0;
            break;
    }
       
    width = static_cast<float>(this->width);
    height = static_cast<float>(this->height);
    x0 = (region.x + x0_offset) / width;
    y0 = (region.y) / height;
    // texture must be flipped due to atlas (0,0) in top left convention
    return {x0+(TextureAtlas::TEX_SIZE)/width, y0+(TextureAtlas::TEX_SIZE)/height, x0, y0};
}

unsigned int vb::TextureAtlas::getTex() {
    return texture;
}