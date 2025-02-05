#ifndef VB_WORLD_HPP
#define VB_WORLD_HPP

#include <unordered_map>
#include <glm/glm.hpp>

#include "core/chunk.hpp"
#include "util/ring_buffer.hpp"

namespace vb {
    
class World {
public:
    World();
    ~World();

    void update();

    const std::unordered_map<ChunkPos, Chunk, ChunkPosHash>& getChunks() const {return chunks;};
private:
    std::unordered_map<ChunkPos, Chunk, ChunkPosHash> chunks;
    vb::RingBuffer<Chunk*, 14> chunkQueue;
};

}


#endif