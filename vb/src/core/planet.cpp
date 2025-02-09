#include "core/planet.hpp"

#include "spdlog/spdlog.h"
#include "glm/gtc/matrix_transform.hpp"

vb::PlanetPos::PlanetPos(int64_t x, int64_t y, int64_t z) : glm::i64vec3(x,y,z) {
    
}

vb::Planet::Planet() {
    // uint16_t i = 0;
    // for (int8_t x = -1; x <= 1; x++) {
    //     for (int8_t z = -1; z <= 1; z++) {
            loaded_chunks[PlanetPos(0, 0, 0)] = new Chunk();
        //     i++;

        // }
    // }
}

vb::Planet::~Planet() {
    for (auto it = loaded_chunks.begin(); it != loaded_chunks.end();) {
        Chunk* chunk = it->second;
        it = loaded_chunks.erase(it);
        delete chunk;
    }
}

void vb::Planet::update() {
    // unload chunks out of range

    // load chunks within range

    // update the loaded chunks
    for (auto& [pos,chunk] : loaded_chunks) {
        if (chunk == nullptr) {
            continue;
        }
        chunk->update();
    }
}

void vb::Planet::render() {
    // render all chunks
    for (auto& [pos,chunk] : loaded_chunks) {
        if (chunk == nullptr) {
            continue;
        }
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(pos.x, pos.y, pos.z));
        // shader->setMat4("model", &model);

        chunk->render();
    }
}

void vb::Planet::setChunkDefaultShader(Shader* shader) {
    this->shader = shader;
    for (auto& [pos,chunk] : loaded_chunks) {
        if (chunk == nullptr) {
            continue;
        }
        chunk->setDefaultShader(shader);
    }
}