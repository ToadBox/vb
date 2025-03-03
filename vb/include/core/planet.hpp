#ifndef VB_PLANET_HPP
#define VB_PLANET_HPP

#include "util/ring_buffer.hpp"
#include "core/chunk.hpp"

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

    void setDefaultShader(Shader* const shader);

    float getTemperature(const PlanetPos& pos);
    float getHumidity(const PlanetPos& pos);

    Chunk* getChunkAt(const PlanetPos& pos);
private:
    // RingBuffer<Chunk, 16> loaded_chunks;
    std::unordered_map<PlanetPos, Chunk*, PlanetPosHash> loaded_chunks;
    Shader* shader;

    uint64_t seed;
};

}

#endif