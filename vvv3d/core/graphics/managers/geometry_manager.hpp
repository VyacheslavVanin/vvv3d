#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <vvv3d/core/graphics/geometry.hpp>

namespace vvv3d {
class GeometryManager {
public:
    using init_func_t = std::function<std::unique_ptr<Geometry>(void)>;

    GeometryManager();
    void add(const std::string& name, std::unique_ptr<Geometry> geom);
    void add(const std::string& name, const init_func_t& f);
    const Geometry& get(const std::string& name) const;
    const Geometry& get(const std::string& name);
    std::vector<std::string> listNames() const;
    void clear();
    bool contain(const std::string& name) const;

private:
    std::unordered_map<std::string, std::unique_ptr<Geometry>> geometries;
    std::unordered_map<std::string, init_func_t> initializers;
};
} // namespace vvv3d
