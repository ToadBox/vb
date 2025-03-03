#include "core/biome_registry.hpp"

bool vb::Biomes::registerBiome(Biome&& biome, uint64_t ID, const TextureRegion& region) {
    
}
bool vb::Biomes::registerBiome(Biome&& biome, ByID ID, const TextureRegion& region) {
    return registerBiome(std::move(biome), (uint64_t)ID, region);
}
const vb::Biome* vb::Biomes::getBiomeByID(uint64_t ID) {
    return biome_registry.getObject(ID);
}
void vb::Biomes::registerBiomes() {

}