#ifndef VB_REGISTRIES_HPP
#define VB_REGISTRIES_HPP

#include <concepts>

#include "core/blocks.hpp"
#include "core/registry_object.hpp"

#include <functional>
#include <memory>

namespace vb {

template <typename T>
requires std::derived_from<T, RegistryObject>
class Registry {
public:
    bool registerObject(T&& obj, uint64_t ID) {
        if (_registry.contains(ID)) {
            return false;
        }
        _registry[ID] = std::move(obj);
        return true;
    };

    const T* getObject(uint64_t ID) {
        if (_registry.contains(ID)) {
            return &_registry.at(ID);
        }

        return nullptr;
    }
private:
    std::unordered_map<uint64_t, T, RegistryObjectHash> _registry = {};
};

}

#endif