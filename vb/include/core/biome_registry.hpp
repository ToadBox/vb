#ifndef VB_BIOME_REGISTRY_HPP
#define VB_BIOME_REGISTRY_HPP

#include <cstdint>
#include <unordered_map>

#include "core/registry.hpp"
#include "core/biome.hpp"
#include "core/texture_region.hpp"

namespace vb {

class Biomes {
public:
    enum class ByID : uint64_t {
        AIR         = 0,
        DIRT        = 1,
        GRASS       = 2,
        STONE       = 3
    };

    static Biomes& get() {
        static Biomes instance;
        return instance;
    }

    bool registerBiome(Biome&& biome, uint64_t ID, const TextureRegion& region);
    bool registerBiome(Biome&& biome, ByID ID, const TextureRegion& region);
    
    const Biome* getBiomeByID(uint64_t ID);

    void registerBiomes();

private:
    Biomes() = default;

    vb::Registry<vb::Biome> biome_registry;
};

}

#endif