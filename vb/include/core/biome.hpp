#ifndef VB_BIOME_HPP
#define VB_BIOME_HPP

#include <string>

#include "core/registry_object.hpp"
#include "glm/vec3.hpp"

namespace vb {

class Biome : public RegistryObject {
public:
    Biome() : RegistryObject("Unnamed_Block") {};
    Biome(const std::string& name, float humidity, float temperature) : 
        RegistryObject(name), humidity(humidity), temperature(temperature) {};
    Biome(const vb::Biome& other) : RegistryObject(other) {
        humidity = other.humidity;
        temperature = other.temperature;
    };
    Biome(vb::Biome&& other) : RegistryObject(std::move(other)) {
        humidity = other.humidity;
        temperature = other.temperature;
    };

    Biome& operator=(const Biome& other) {
        if (this != &other) {
            RegistryObject::operator=(other);
            temperature = other.temperature;
            humidity = other.humidity;
        }
        return *this;
    }
private:
    glm::vec3 foliage_color;
    glm::vec3 water_color;
    float humidity;
    float temperature;
};

}

#endif