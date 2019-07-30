#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <vvv3d/core/graphics/shaders/shader.hpp>

namespace vvv3d {
class ShaderManager {
public:
    ShaderManager();

    void add(const std::string& name, const std::string& vertexShaderFilename,
             const std::string& fragmentShaderFilename);
    void add(const std::string& name, const std::string& vertexShaderFilename,
             const std::string& fragmentShaderFilename,
             const std::string& geometryShaderFilename);
    void addFromSource(const std::string& name,
                       const std::string& vertexShaderSource,
                       const std::string& fragmentShaderSource);
    void addFromSource(const std::string& name,
                       const std::string& vertexShaderSource,
                       const std::string& fragmentShaderSource,
                       const std::string& geometryShaderSource);

    void add(const std::string& name, std::unique_ptr<Shader> shader);
    void add(const std::string& name,
             const std::function<std::unique_ptr<Shader>()>& f);
    Shader& get(const std::string& name) const;
    Shader& get(const std::string& name);
    bool contain(const std::string& name) const;

    std::vector<std::string> listNames() const;

private:
    std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;
    using init_func_t = std::function<std::unique_ptr<Shader>(void)>;
    std::unordered_map<std::string, init_func_t> initializers;
};
} // namespace vvv3d
