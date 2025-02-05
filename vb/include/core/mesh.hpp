#ifndef VB_MESH_HPP
#define VB_MESH_HPP

#include <cstdint>
#include <vector>

namespace vb {

struct MeshVertex {
    float x, y, z;
    float u, v;
};

struct Mesh {
    std::vector<MeshVertex> vertices;
    std::vector<uint32_t> indices;
};

}

#endif