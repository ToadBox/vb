#include "core/chunk.hpp"
#include "core/planet.hpp"
#include "core/world_gen.hpp"

#include "glad/gl.h"

#include "spdlog/spdlog.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

vb::ChunkPos::ChunkPos(int32_t x, int32_t y, int32_t z) : glm::i32vec3(x,y,z) {

}

vb::ChunkData::ChunkData(Chunk* chunk) {
    this->chunk = chunk;
}

bool vb::ChunkData::isBlockAt(const ChunkPos& pos) const {
    if (!isBlockWithinBounds(pos)) {
        ChunkPos modified_chunk_pos = pos;
        PlanetPos adjacent_chunk_pos = chunk->pos;
        if (pos.x >= vb::Chunk::SIZE) {
            adjacent_chunk_pos += glm::i64vec3(1, 0, 0);
        }
        if (pos.y >= vb::Chunk::SIZE) {
            adjacent_chunk_pos += glm::i64vec3(0, 1, 0);
        }
        if (pos.z >= vb::Chunk::SIZE) {
            adjacent_chunk_pos += glm::i64vec3(0, 0, 1);
        }
        if (pos.x < 0) {
            adjacent_chunk_pos -= glm::i64vec3(1, 0, 0);
        } 
        if (pos.y < 0) {
            adjacent_chunk_pos -= glm::i64vec3(0, 1, 0);
        }
        if (pos.z < 0) {
            adjacent_chunk_pos -= glm::i64vec3(0, 0, 1);
        }
        modified_chunk_pos.x = (modified_chunk_pos.x + vb::Chunk::SIZE) % vb::Chunk::SIZE;
        modified_chunk_pos.y = (modified_chunk_pos.y + vb::Chunk::SIZE) % vb::Chunk::SIZE;
        modified_chunk_pos.z = (modified_chunk_pos.z + vb::Chunk::SIZE) % vb::Chunk::SIZE;
        
        Chunk* adjacent = chunk->planet->getChunkAt(adjacent_chunk_pos);
        if (adjacent == nullptr) {
            return false;
        }

        // spdlog::debug("{}, {}", glm::to_string((glm::i64vec3)modified_chunk_pos), adjacent->data.isBlockAt(modified_chunk_pos));
        
        return adjacent->data.isBlockAt(modified_chunk_pos);
    }
    
    return blocks.count(pos);
}

bool vb::ChunkData::isBlockWithinBounds(const ChunkPos& pos) const {
    if (pos.x >= Chunk::SIZE || pos.y >= Chunk::SIZE || pos.z >= Chunk::SIZE ||
        pos.x < 0 || pos.y < 0 || pos.z < 0) {
            return false;
    }

    return true;
}

void vb::ChunkData::set(const ChunkPos& pos, uint64_t blockID) {
    blocks[pos] = blockID;
    assert(isBlockAt(pos));
}
 
void vb::ChunkData::set(const ChunkPos& pos, Blocks::ByID blockID) {
    set(pos, (uint64_t) blockID);
}

vb::ChunkMesh::ChunkMesh(Chunk* chunk) {
    this->chunk = chunk;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, x));
    glEnableVertexAttribArray(0); 
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, u));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, biome_r));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

vb::ChunkMesh::~ChunkMesh() {
    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

vb::Chunk::Chunk(Planet* planet, const PlanetPos& pos, WorldGenerator* gen) : mesh(this), data(this) {
    this->planet = planet;
    this->pos = pos;
    dirty = true;
    atlas = &vb::TextureAtlas::getAtlas();
    overlay_atlas = &vb::TextureOverlayAtlas::getAtlas();
    shader = nullptr;
    gen->generateChunk(&data);
}

vb::Chunk::~Chunk() {

}

void vb::Chunk::update() {
    if (dirty) {
        build();
    }
}

void vb::Chunk::render() {
    glBindVertexArray(mesh.VAO);
    shader->use();
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlas->getTex());
    shader->setInt("baseTex", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, overlay_atlas->getTex());
    shader->setInt("overlayTex", 1);

    glm::vec3 biome_color{0, 0.342, 0.071};
    shader->setVec3("biome", &biome_color);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, pos.z) * static_cast<float>(vb::Chunk::SIZE));
    shader->setMat4("model", &model);

    glDrawElements(GL_TRIANGLES, mesh.face_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void vb::Chunk::setDefaultShader(Shader* const shader) {
    this->shader = shader;
}

void vb::Chunk::build() {
    // TODO: greedy mesh?
    for (auto& [chunkpos, blockid] : data.blocks) {
        NormalizedTextureRegion region;
        uint8_t num_faces_added = 0;
        glm::vec3 pos = glm::vec3(chunkpos);

        glm::vec3 biome_color = glm::vec3(1.0, 1.0, 1.0);

        if (blockid == static_cast<uint64_t>(Blocks::ByID::GRASS)) {
            // TODO: get biome at block
            biome_color = glm::vec3(0.486, 0.741, 0.42);
        }
        
        // TOP
        if (!data.isBlockAt(ChunkPos(pos.x, pos.y+1, pos.z))) {
            region = TextureAtlas::getAtlas().getNormalizedTexCoords(blockid, Face::TOP);
            MeshVertex Top[4] = {
                pos.x+1.0f, pos.y+1.0f, pos.z+1.0f, region.x1, region.y1, biome_color.r, biome_color.g, biome_color.b,
                pos.x,      pos.y+1.0f, pos.z+1.0f, region.x1, region.y0, biome_color.r, biome_color.g, biome_color.b,
                pos.x,      pos.y+1.0f, pos.z,      region.x0, region.y0, biome_color.r, biome_color.g, biome_color.b,
                pos.x+1.0f, pos.y+1.0f, pos.z,      region.x0, region.y1, biome_color.r, biome_color.g, biome_color.b
            };
            mesh.face_vertices.insert(mesh.face_vertices.end(), Top, Top+4);
            num_faces_added++;
        }
        // SOUTH
        if (!data.isBlockAt(ChunkPos(pos.x-1, pos.y, pos.z))) {
            region = TextureAtlas::getAtlas().getNormalizedTexCoords(blockid, Face::SOUTH);
            MeshVertex South[4] = {
                pos.x, pos.y+1.0f,  pos.z+1.0f,     region.x1, region.y1, biome_color.r, biome_color.g, biome_color.b,
                pos.x, pos.y,       pos.z+1.0f,     region.x1, region.y0, 1.0, 1.0, 1.0,
                pos.x, pos.y,       pos.z,          region.x0, region.y0, 1.0, 1.0, 1.0,
                pos.x, pos.y+1.0f,  pos.z,          region.x0, region.y1, biome_color.r, biome_color.g, biome_color.b,
            };
            mesh.face_vertices.insert(mesh.face_vertices.end(), South, South+4);
            num_faces_added++;
        }
        // EAST
        if (!data.isBlockAt(ChunkPos(pos.x, pos.y, pos.z+1))) {
            region = TextureAtlas::getAtlas().getNormalizedTexCoords(blockid, Face::EAST);
            MeshVertex East[4]  = {
                pos.x+1.0f, pos.y+1.0f, pos.z+1.0f, region.x1, region.y1, biome_color.r, biome_color.g, biome_color.b,
                pos.x+1.0f, pos.y,      pos.z+1.0f, region.x1, region.y0, 1.0, 1.0, 1.0,
                pos.x,      pos.y,      pos.z+1.0f, region.x0, region.y0, 1.0, 1.0, 1.0,
                pos.x,      pos.y+1.0f, pos.z+1.0f, region.x0, region.y1, biome_color.r, biome_color.g, biome_color.b,
            };
            mesh.face_vertices.insert(mesh.face_vertices.end(), East, East+4);
            num_faces_added++;
        }
        // WEST
        if (!data.isBlockAt(ChunkPos(pos.x, pos.y, pos.z-1))) {
            region = TextureAtlas::getAtlas().getNormalizedTexCoords(blockid, Face::WEST);
            MeshVertex West[4]  = {
                pos.x,      pos.y+1.0f, pos.z,      region.x1, region.y1, biome_color.r, biome_color.g, biome_color.b,
                pos.x,      pos.y,      pos.z,      region.x1, region.y0, 1.0, 1.0, 1.0,
                pos.x+1.0f, pos.y,      pos.z,      region.x0, region.y0, 1.0, 1.0, 1.0,
                pos.x+1.0f, pos.y+1.0f, pos.z,      region.x0, region.y1, biome_color.r, biome_color.g, biome_color.b,
            };
            
            mesh.face_vertices.insert(mesh.face_vertices.end(), West, West+4);
            num_faces_added++;
        }
        // NORTH
        if (!data.isBlockAt(ChunkPos(pos.x+1, pos.y, pos.z))) {
            region = TextureAtlas::getAtlas().getNormalizedTexCoords(blockid, Face::NORTH);
            MeshVertex North[4]  = {
                pos.x+1.0f, pos.y+1.0f, pos.z+1.0f, region.x0, region.y1, biome_color.r, biome_color.g, biome_color.b,
                pos.x+1.0f, pos.y+1.0f, pos.z,      region.x1, region.y1, biome_color.r, biome_color.g, biome_color.b,
                pos.x+1.0f, pos.y,      pos.z,      region.x1, region.y0, 1.0, 1.0, 1.0,
                pos.x+1.0f, pos.y,      pos.z+1.0f, region.x0, region.y0, 1.0, 1.0, 1.0,
            };
            mesh.face_vertices.insert(mesh.face_vertices.end(), North, North+4);
            num_faces_added++;
        }
        // BOT
        if (!data.isBlockAt(ChunkPos(pos.x, pos.y-1, pos.z))) {
            region = TextureAtlas::getAtlas().getNormalizedTexCoords(blockid, Face::BOT);
            MeshVertex Bot[4]  = {
                pos.x+1.0f, pos.y, pos.z+1.0f,      region.x1, region.y0, 1.0, 1.0, 1.0,
                pos.x+1.0f, pos.y, pos.z,           region.x0, region.y0, 1.0, 1.0, 1.0,
                pos.x,      pos.y, pos.z,           region.x0, region.y1, 1.0, 1.0, 1.0,
                pos.x,      pos.y, pos.z+1.0f,      region.x1, region.y1, 1.0, 1.0, 1.0,
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

        if(num_faces_added > 0) {
            mesh.num_tri += num_faces_added * 2;
            // spdlog::warn("{},{},{},{}", region.x0, region.x1, region.y0, region.y1);
        }
    }
    
    // for (auto& v : mesh.face_vertices) {
    //     spdlog::warn("{},{},{},{},{}", v.x, v.y, v.z, v.u, v.v);
    // }
    // for (uint64_t i = 0; i < mesh.face_indices.size(); i+=3) {
    //     spdlog::warn("{},{},{}", mesh.face_indices[i], mesh.face_indices[i+1], mesh.face_indices[i+2]);
    // }

    // spdlog::info("Vertices: {}, Indices: {}", mesh.face_vertices.size(), mesh.face_indices.size());
    // spdlog::info(mesh.num_tri);

    glBindVertexArray(mesh.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(MeshVertex)*mesh.face_vertices.size(), mesh.face_vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t)*mesh.face_indices.size(), mesh.face_indices.data(), GL_STATIC_DRAW);
    dirty = false;
}