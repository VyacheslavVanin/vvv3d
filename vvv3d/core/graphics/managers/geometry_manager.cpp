#include "geometry_manager.hpp"

#include <vvv3d/std/log.hpp>

namespace vvv3d {
using ::vvv::helper::format;

GeometryManager::GeometryManager() : geometries() {}

void GeometryManager::add(const std::string& name,
                          std::unique_ptr<Geometry> geom)
{
    if (contain(name))
        return;

    geometries[name].swap(geom);
}

void GeometryManager::add(const std::string& name,
                          const GeometryManager::init_func_t& f)
{
    initializers[name] = f;
}

const Geometry& GeometryManager::get(const std::string& name) const
try {
    return *geometries.at(name);
}
catch (const std::exception& e) {
    LOG_ERROR(format("failed to get geometry \"@\"", name));
    throw;
}

const Geometry& GeometryManager::get(const std::string& name)
try {
    if (!contain(name)) {
        auto it = initializers.find(name);
        if (it != initializers.end())
            add(name, it->second());
    }
    return *geometries.at(name);
}
catch (const std::exception& e) {
    LOG_ERROR(format("failed to get geometry \"@\"", name));
    throw;
}

bool GeometryManager::contain(const std::string& name) const
{
    return geometries.find(name) != geometries.end();
}

void GeometryManager::clear() { geometries.clear(); }

std::vector<std::string> GeometryManager::listNames() const
{
    std::vector<std::string> ret;
    for (const auto& kv : geometries)
        ret.push_back(kv.first);
    return ret;
}
} // namespace vvv3d
