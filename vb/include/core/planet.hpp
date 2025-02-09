#ifndef VB_PLANET_HPP
#define VB_PLANET_HPP

#include "util/ring_buffer.hpp"
#include "core/chunk2.hpp"

#include <array>
#include "glm/vec3.hpp"


namespace vb {
class Chunk;

class Planet {
public:
    Planet();
    Planet(const Planet&) = delete;
    ~Planet();
    Planet& operator=(const Planet&) = delete;
    void update();
    void render();

    void setChunkDefaultShader(Shader* shader);
    Chunk* getChunkAt(const PlanetPos& pos);
private:
    // RingBuffer<Chunk, 16> loaded_chunks;
    std::unordered_map<PlanetPos, Chunk*, PlanetPosHash> loaded_chunks;
    Shader* shader;
};

}

#endif