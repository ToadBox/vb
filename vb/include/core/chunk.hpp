#ifndef VB_CHUNK_HPP
#define VB_CHUNK_HPP

#include <cstdint>
#include <functional>

#include "core/blocks.hpp"
#include "core/mesh.hpp"

#include "glm/glm.hpp"

namespace vb {

class Chunk;
class World;

struct ChunkPos {
    ChunkPos(const int64_t x=0, const int64_t y=0, const int64_t z=0);
    ChunkPos(const glm::i64vec3& pos);
    ChunkPos(const ChunkPos& other);
    int64_t x;
    int64_t y;
    int64_t z;

    glm::i64vec3 inline asVec3() {return glm::i64vec3(x, y, z);};
    bool operator==(const ChunkPos& other) const;
    ChunkPos& operator=(const ChunkPos& other);
};

struct ChunkPosHash {
    size_t operator()(const ChunkPos& v) const {
        return (std::hash<int64_t>()(v.x) * 73856093) ^ 
               (std::hash<int64_t>()(v.y) * 47194333) ^ 
               (std::hash<int64_t>()(v.z) * 83492791);
    }
};

struct InChunkPos {
    InChunkPos(const uint8_t x=0, const uint8_t y=0, const uint8_t z=0);
    InChunkPos(const glm::u8vec3& pos);
    InChunkPos(const InChunkPos& other);
    uint8_t x;
    uint8_t y;
    uint8_t z;

    glm::u8vec3 inline asVec3() {return glm::u8vec3(x, y, z);};
    bool inline isValidPos() const;
    bool operator==(const InChunkPos& other) const;
    InChunkPos& operator=(const InChunkPos& other);
};

struct InChunkPosHash {
    size_t operator()(const InChunkPos& v) const {
        return (std::hash<uint8_t>()(v.x) * 73856093) ^ 
               (std::hash<uint8_t>()(v.y) * 47194333) ^ 
               (std::hash<uint8_t>()(v.z) * 83492791);
    }
};

class ChunkData {
public:
    // getter
    uint64_t at(const InChunkPos& pos) const;

    // setter
    void set(const InChunkPos& pos, uint64_t blockID);

private:
    std::unordered_map<InChunkPos, uint64_t, InChunkPosHash> blocks;
friend Chunk;
};

class Chunk {
public:
    static constexpr uint8_t SIZE = 16;
    static_assert(SIZE > 0);
    Chunk();
    Chunk(ChunkPos pos);
    ~Chunk();
    
    const ChunkData& getData() const {return data;};

    void updateChunk();

    uint64_t getBlockAt(const glm::u8vec3& pos) const;
    uint64_t getBlockAt(const uint8_t x, const uint8_t y, const uint8_t z) const;

    void markDirty();

    Chunk& operator=(const Chunk& other);
private:
    void buildMesh();

    ChunkData data;
    ChunkPos pos;
    Mesh mesh;

    bool dirty;
};

}

#endif