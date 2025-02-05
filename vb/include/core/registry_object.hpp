#ifndef VB_REGISTRY_OBJECT_HPP
#define VB_REGISTRY_OBJECT_HPP

#include <cstdint>
#include <string>

namespace vb {

struct RegistryObjectHash {
    size_t operator()(const uint64_t& ID) const {
        return std::hash<uint64_t>()(ID);
    }
};

class RegistryObject {
public:
    RegistryObject(const std::string& name) : name(name) {};
    virtual ~RegistryObject() = default;
    const inline std::string& GetName() const {return name;};
private:
    uint64_t ID;
    std::string name;
};

}

#endif