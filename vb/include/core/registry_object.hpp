#ifndef VB_REGISTRY_OBJECT_HPP
#define VB_REGISTRY_OBJECT_HPP

#include <cstdint>
#include <string>

namespace vb {

class RegistryObject {
public:
    uint64_t inline GetID() {return ID;};
    std::string inline GetName() {return name;};
private:
    uint64_t ID;
    std::string name;
};

}

#endif