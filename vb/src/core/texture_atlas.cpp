#include "core/texture_atlas.hpp"
#include "stb_image.h"
#include "glad/gl.h"
#include "spdlog/spdlog.h"

vb::TextureAtlas::TextureAtlas() {
    
}

void vb::TextureAtlas::bindTextureRegion(const uint64_t ID, const TextureRegion& region) {
    atlas[ID] = region;
}

void vb::TextureAtlas::bindTextureRegion(const uint64_t ID, uint64_t x, uint64_t y, uint64_t width, uint64_t height) {
    atlas[ID] = {x,y,width,height};
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        spdlog::error("Failed to load texture atlas {}", loc);
    }
    stbi_image_free(data);
}

vb::NormalizedTextureRegion vb::TextureAtlas::getNormalizedTexCoords(const uint64_t ID) {
    TextureRegion region = atlas[ID];
    float width = static_cast<float>(this->width);
    float height = static_cast<float>(this->height);
    float x0 = (region.x) / width;
    float y0 = (region.y) / height;
    return {x0, y0, x0+(region.x+region.width)/width, y0+(region.y+region.height)/height};
}

unsigned int vb::TextureAtlas::getTex() {
    return texture;
}