#ifndef VB_CHUNK2_HPP
#define VB_CHUNK2_HPP

#include <cstdint>
#include <vector>
#include <unordered_map>
#include "core/texture_atlas.hpp"
#include "core/planet_pos.hpp"
#include "core/block_registry.hpp"
#include "core/shader.hpp"

#include "glm/vec3.hpp"
#include "spdlog/spdlog.h"

namespace vb {
class Chunk;
class Planet;

struct ChunkPos : public glm::i32vec3 {
    ChunkPos() = default;
    ChunkPos(int32_t x, int32_t y, int32_t z);
};

struct ChunkPosHash {
    size_t operator()(const ChunkPos& v) const {
        size_t hash = ((31 + std::hash<int32_t>()(v.x)) * 
                (31 + std::hash<int32_t>()(v.y)) *
                (31 + std::hash<int32_t>()(v.z))) * 17;
        // spdlog::debug(hash);
        return hash;
    }
};

struct MeshVertex {
    float x,y,z;
    float u,v;
};

class ChunkData {
public:
    ChunkData(Chunk* chunk);
    bool isBlockAt(const ChunkPos& pos) const;
    bool isBlockWithinBounds(const ChunkPos& pos) const;

    void set(const ChunkPos& pos, uint64_t blockID);
    void set(const ChunkPos& pos, Blocks::ByID ID);
private:
    std::unordered_map<ChunkPos, uint64_t, ChunkPosHash> blocks;
    // std::unordered_map<ChunkPos, uint64_t, ChunkPosHash> foliage_blocks;
    // std::unordered_map<ChunkPos, uint64_t, ChunkPosHash> biome_overrides;
    uint64_t biome_id;
    Chunk* chunk;
friend Chunk;
};

class ChunkMesh {
public:
    ChunkMesh(Chunk* chunk);
    ChunkMesh(const ChunkMesh&) = delete;
    ~ChunkMesh();
    ChunkMesh& operator=(const ChunkMesh&) = delete;
private:
    std::vector<MeshVertex> face_vertices;
    std::vector<uint32_t> face_indices;
    uint32_t num_tri = 0;
    unsigned int VAO=0, VBO=0, EBO=0;
    Chunk* chunk;
friend Chunk;
};

class Chunk {
public:
    static constexpr uint8_t SIZE = 16;
    static_assert(SIZE > 0);
    Chunk(Planet* planet, const PlanetPos& pos);
    Chunk(const Chunk&) = delete;
    ~Chunk();
    Chunk& operator=(const Chunk&) = delete;

    void update();
    void render();

    void setDefaultShader(Shader* const shader);
private:
    void build();

    ChunkMesh mesh;
    ChunkData data;
    TextureAtlas* atlas;
    Planet* planet;
    Shader* shader;
    PlanetPos pos;

    bool dirty;
friend ChunkMesh;
friend ChunkData;
};

}

#endif