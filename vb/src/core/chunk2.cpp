#include "core/chunk2.hpp"

#include "glad/gl.h"

#include "spdlog/spdlog.h"

vb::ChunkPos::ChunkPos(int32_t x, int32_t y, int32_t z) : glm::i32vec3(x,y,z) {

}

bool inline vb::ChunkData::isBlockAt(const ChunkPos& pos) const {
    return blocks.count(pos);
}

void vb::ChunkData::set(const ChunkPos& pos, uint64_t blockID) {
    blocks[pos] = blockID;
}
 
void vb::ChunkData::set(const ChunkPos& pos, Blocks::ByID blockID) {
    blocks[pos] = (uint64_t) blockID;
}

vb::ChunkMesh::ChunkMesh() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, x));
    glEnableVertexAttribArray(0); 
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, u));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

vb::ChunkMesh::~ChunkMesh() {
    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

vb::Chunk::Chunk() {
    dirty = true;
    atlas = &vb::TextureAtlas::getAtlas();

    // place layer of grass at y=15
    for (uint8_t x = 0; x < Chunk::SIZE; x++) {
        for (uint8_t z = 0; z < Chunk::SIZE; z++) {
            data.set(ChunkPos(x, SIZE-1, z), Blocks::ByID::GRASS);
        }
    }
    // // place 5 layers of dirt
    // for (uint8_t x = 0; x < Chunk::SIZE; x++) {
    //     for (uint8_t z = 0; z < Chunk::SIZE; z++) {
    //         for (int8_t y = SIZE-2; y > SIZE-8; y--) {
    //             data.set(ChunkPos(x, y, z), Blocks::ByID::DIRT);
    //         }
    //     }
    // }
    // // place 5 layers of dirt
    // for (uint8_t x = 0; x < Chunk::SIZE; x++) {
    //     for (uint8_t z = 0; z < Chunk::SIZE; z++) {
    //         for (int8_t y = SIZE-8; y >= 0; y--) {
    //             data.set(ChunkPos(x, y, z), Blocks::ByID::STONE);
    //         }
    //     }
    // }
}

vb::Chunk::~Chunk() {

}

void vb::Chunk::setDefaultShader(Shader* shader) {
    this->shader = shader;
}

void vb::Chunk::update() {
    if (dirty) {
        build();
    }
    
}

void vb::Chunk::render() {
    glBindVertexArray(mesh.VAO);
    glBindTexture(GL_TEXTURE_2D, atlas->getTex());
    glDrawElements(GL_TRIANGLES, mesh.num_tri, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void vb::Chunk::build() {
    // TODO: greedy mesh?
    for (auto& [pos, blockid] : data.blocks) {
        NormalizedTextureRegion region = TextureAtlas::getAtlas().getNormalizedTexCoords(blockid);
        uint8_t num_faces_added = 0;

        spdlog::critical("{},{},{},{}", region.x0, region.y0, region.x1, region.y1);
        
        // TOP
        if (!data.isBlockAt(ChunkPos(pos.x, pos.y+1, pos.z))) {
            MeshVertex Top[4] = {
                pos.x+1.0f, pos.y+1.0f, pos.z+1.0f, region.x1, region.y1,
                static_cast<float>(pos.x), pos.y+1.0f, pos.z+1.0f, region.x1, region.y0,
                static_cast<float>(pos.x), pos.y+1.0f, static_cast<float>(pos.z), region.x0, region.y0,
                pos.x+1.0f, pos.y+1.0f, static_cast<float>(pos.z), region.x0, region.y1
            };
            mesh.face_vertices.insert(mesh.face_vertices.end(), Top, Top+4);
            num_faces_added++;
        }
        // SOUTH
        if (!data.isBlockAt(ChunkPos(pos.x-1, pos.y, pos.z))) {
            MeshVertex South[4] = {
                static_cast<float>(pos.x), pos.y+1.0f, pos.z+1.0f, region.x1, region.y1,
                static_cast<float>(pos.x), static_cast<float>(pos.y), pos.z+1.0f, region.x1, region.y0,
                static_cast<float>(pos.x), static_cast<float>(pos.y), static_cast<float>(pos.z), region.x0, region.y0,
                static_cast<float>(pos.x), pos.y+1.0f, static_cast<float>(pos.z), region.x0, region.y1
            };
            mesh.face_vertices.insert(mesh.face_vertices.end(), South, South+4);
            num_faces_added++;
        }
        // EAST
        if (!data.isBlockAt(ChunkPos(pos.x, pos.y, pos.z+1))) {
            MeshVertex East[4]  = {
                pos.x+1.0f, pos.y+1.0f, pos.z+1.0f, region.x1, region.y1,
                pos.x+1.0f, static_cast<float>(pos.y), pos.z+1.0f, region.x1, region.y0,
                static_cast<float>(pos.x), static_cast<float>(pos.y), pos.z+1.0f, region.x0, region.y0,
                static_cast<float>(pos.x), pos.y+1.0f, pos.z+1.0f, region.x0, region.y1
            };
            mesh.face_vertices.insert(mesh.face_vertices.end(), East, East+4);
            num_faces_added++;
        }
        // WEST
        if (!data.isBlockAt(ChunkPos(pos.x, pos.y, pos.z-1))) {
            MeshVertex West[4]  = {
                static_cast<float>(pos.x), pos.y+1.0f, static_cast<float>(pos.z), region.x1, region.y1,
                static_cast<float>(pos.x), static_cast<float>(pos.y), static_cast<float>(pos.z), region.x1, region.y0,
                pos.x+1.0f, static_cast<float>(pos.y), static_cast<float>(pos.z), region.x0, region.y0,
                pos.x+1.0f, pos.y+1.0f, static_cast<float>(pos.z), region.x0, region.y1
            };
            
            mesh.face_vertices.insert(mesh.face_vertices.end(), West, West+4);
            num_faces_added++;
        }
        // NORTH
        if (!data.isBlockAt(ChunkPos(pos.x+1, pos.y, pos.z))) {
            MeshVertex North[4]  = {
                pos.x+1.0f, pos.y+1.0f, pos.z+1.0f, region.x1, region.y1,
                pos.x+1.0f, static_cast<float>(pos.y), pos.z+1.0f, region.x1, region.y0,
                pos.x+1.0f, pos.y+1.0f, static_cast<float>(pos.z), region.x0, region.y0,
                pos.x+1.0f, static_cast<float>(pos.y), static_cast<float>(pos.z), region.x0, region.y1
            };
            mesh.face_vertices.insert(mesh.face_vertices.end(), North, North+4);
            num_faces_added++;
        }
        // BOT
        if (!data.isBlockAt(ChunkPos(pos.x, pos.y-1, pos.z))) {
            MeshVertex Bot[4]  = {
                pos.x+1.0f, static_cast<float>(pos.y), pos.z+1.0f, region.x1, region.y1,
                pos.x+1.0f, static_cast<float>(pos.y), static_cast<float>(pos.z), region.x1, region.y0,
                static_cast<float>(pos.x), static_cast<float>(pos.y), static_cast<float>(pos.z), region.x0, region.y0,
                static_cast<float>(pos.x), static_cast<float>(pos.y), pos.z+1.0f, region.x0, region.y1
            };
            mesh.face_vertices.insert(mesh.face_vertices.end(), Bot, Bot+4);
            num_faces_added++;
        }

        for (uint8_t i = 0; i < num_faces_added; i++) {
            uint32_t idx_offset = 2*(mesh.face_indices.size()/3);
            unsigned int indices[] = {
                idx_offset+0, idx_offset+1, idx_offset+2, idx_offset+0, idx_offset+2, idx_offset+3
            };
            mesh.face_indices.insert(mesh.face_indices.end(), indices, indices+6);
        }

        mesh.num_tri += num_faces_added * 2;
    }
    
    // for (auto& v : mesh.face_vertices) {
    //     spdlog::warn("{},{},{},{},{}", v.x, v.y, v.z, v.u, v.v);
    // }
    // for (uint64_t i = 0; i < mesh.face_indices.size(); i+=3) {
    //     spdlog::warn("{},{},{}", mesh.face_indices[i], mesh.face_indices[i+1], mesh.face_indices[i+2]);
    // }

    spdlog::info("Vertices: {}, Indices: {}", mesh.face_vertices.size(), mesh.face_indices.size());

    glBindVertexArray(mesh.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(MeshVertex)*mesh.face_vertices.size(), mesh.face_vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t)*mesh.face_indices.size(), mesh.face_indices.data(), GL_STATIC_DRAW);
    dirty = false;
}