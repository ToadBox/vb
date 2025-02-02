#ifndef VB_REGISTRIES_HPP
#define VB_REGISTRIES_HPP

#include <concepts>

#include "core/blocks.hpp"
#include "core/registry_object.hpp"

#include <memory>

namespace vb {

template <typename T>
requires std::derived_from<T, RegistryObject>
class Registry {
public:
    bool Register(T&& obj, uint64_t ID) {_registry[ID] = obj;};
private:
    std::unordered_map<uint64_t, T> _registry;
};

}

#endif