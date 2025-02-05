#include "core/chunk.hpp"
#include "spdlog/spdlog.h"

#include "glad/gl.h"

vb::ChunkPos::ChunkPos(const int64_t x, const int64_t y, const int64_t z) {
    this->x = x;    
    this->y = y;    
    this->z = z;    
}

vb::ChunkPos::ChunkPos(const glm::i64vec3& pos) {
    this->x = pos.x;    
    this->y = pos.y;    
    this->z = pos.z;  
}

vb::ChunkPos::ChunkPos(const ChunkPos& other) {
    this->x = other.x;    
    this->y = other.y;    
    this->z = other.z;  
}

bool vb::ChunkPos::operator==(const ChunkPos& other) const {
    return (x == other.x) && (y == other.y) && (z == other.z);
}

vb::ChunkPos& vb::ChunkPos::operator=(const ChunkPos& other) {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    return *this;
}

vb::InChunkPos::InChunkPos(const uint8_t x, const uint8_t y, const uint8_t z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

vb::InChunkPos::InChunkPos(const glm::u8vec3& pos) {
    this->x = pos.x;    
    this->y = pos.y;    
    this->z = pos.z;  
}

vb::InChunkPos::InChunkPos(const InChunkPos& other) {
    this->x = other.x;    
    this->y = other.y;    
    this->z = other.z;  
}

bool inline vb::InChunkPos::isValidPos() const {
    return (x > 0 && x < Chunk::SIZE) && (y > 0 && y < Chunk::SIZE) && (z > 0 && z < Chunk::SIZE);
}

bool vb::InChunkPos::operator==(const InChunkPos& other) const {
    return (x == other.x) && (y == other.y) && (z == other.z);
}

vb::InChunkPos& vb::InChunkPos::operator=(const InChunkPos& other) {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    return *this;
}

uint64_t vb::ChunkData::at(const InChunkPos& pos) const {
    assert(pos.isValidPos());
    if (blocks.contains(pos)) {
        return blocks.at(pos);
    }
    
    return (uint64_t) Blocks::ByID::AIR;
}

void vb::ChunkData::set(const InChunkPos& pos, uint64_t blockID) {
    assert(pos.isValidPos());
    blocks[pos] = blockID;
}

vb::Chunk::Chunk() {
    markDirty();
}

vb::Chunk::Chunk(ChunkPos pos) : pos(pos) {

    // TODO: load from memory
    for (uint8_t x = 1; x < SIZE; x++) {
        for (uint8_t z = 1; z < SIZE; z++) {
            for (uint8_t y = 1; y < SIZE; y++) {
                if ((x^z) % 2 == (y%2)) {
                    auto pos = InChunkPos(x,y,z);
                    data.blocks[pos] = (uint64_t) Blocks::ByID::GRASS;
                }
            }
        }
    }
    markDirty();
}

vb::Chunk& vb::Chunk::operator=(const Chunk& other) {
    this->data = other.data;

    this->pos = other.pos;

    this->mesh = other.mesh;

    markDirty();
    return *this;
}

vb::Chunk::~Chunk() {
    // TODO: save the chunk
}

void vb::Chunk::updateChunk() {
    // only update if there is something to do
    if (!dirty) return;
    
    buildMesh();
    dirty = false;
}

void vb::Chunk::buildMesh() {
    spdlog::info("Chunk mesh update");
}

uint64_t vb::Chunk::getBlockAt(const glm::u8vec3& pos) const {
    return data.at(pos);
}

uint64_t vb::Chunk::getBlockAt(const uint8_t x, const uint8_t y, const uint8_t z) const {
    return data.at(glm::u8vec3(x,y,z));
}

void vb::Chunk::markDirty() {
    dirty = true;
}