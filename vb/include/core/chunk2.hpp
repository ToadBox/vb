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

namespace vb {
class Chunk;

struct ChunkPos : public glm::i32vec3 {
    ChunkPos() = default;
    ChunkPos(int32_t x, int32_t y, int32_t z);
};

struct ChunkPosHash {
    size_t operator()(const ChunkPos& v) const {
        return (std::hash<int32_t>()(v.x) * 73856093) ^ 
               (std::hash<int32_t>()(v.y) * 47194333) ^ 
               (std::hash<int32_t>()(v.z) * 83492791);
    }
};

struct MeshVertex {
    float x,y,z;
    float u,v;
};

class ChunkData {
public:
    bool inline isBlockAt(const ChunkPos& pos) const;

    void set(const ChunkPos& pos, uint64_t blockID);
    void set(const ChunkPos& pos, Blocks::ByID ID);
private:
    std::unordered_map<ChunkPos, uint64_t, ChunkPosHash> blocks;
friend Chunk;
};

class ChunkMesh {
public:
    ChunkMesh();
    ChunkMesh(const ChunkMesh&) = delete;
    ~ChunkMesh();
    ChunkMesh& operator=(const ChunkMesh&) = delete;
private:
    std::vector<MeshVertex> face_vertices;
    std::vector<uint32_t> face_indices;
    uint64_t num_tri = 0;
    unsigned int VAO=0, VBO=0, EBO=0;
friend Chunk;
};

class Chunk {
public:
    static constexpr uint8_t SIZE = 1;
    static_assert(SIZE > 0);
    Chunk();
    Chunk(const Chunk&) = delete;
    ~Chunk();
    Chunk& operator=(const Chunk&) = delete;

    void setDefaultShader(Shader* shader);

    void update();
    void render();
private:
    void build();

    ChunkMesh mesh;
    ChunkData data;
    TextureAtlas* atlas;
    Shader* shader;

    bool dirty;
};

}

#endif