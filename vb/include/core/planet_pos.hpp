#ifndef VB_PLANET_POS_HPP
#define VB_PLANET_POS_HPP

#include <cstdint>
#include <functional>
#include "glm/vec3.hpp"

namespace vb {

struct PlanetPos : public glm::i64vec3 {
    PlanetPos() = default;
    PlanetPos(int64_t x, int64_t y, int64_t z);
};

struct PlanetPosHash {
    size_t operator()(const PlanetPos& v) const {
        return (std::hash<int64_t>()(v.x) * 73856093) ^ 
               (std::hash<int64_t>()(v.y) * 47194333) ^ 
               (std::hash<int64_t>()(v.z) * 83492791);
    }
};

}

#endif