#ifndef VB_WORLD_GEN_HPP
#define VB_WORLD_GEN_HPP

#include <cstdint>

namespace vb {
class ChunkData;

class WorldGenerator {
public:
    void temperatureSeed(uint32_t seed);
    void humiditySeed(uint32_t seed);
    void worldSeed(uint32_t seed);
    void generateChunk(ChunkData* const data) const;
private:
    float temperature_seed;
    float humidity_seed;
    float world_seed;
};

};

#endif 