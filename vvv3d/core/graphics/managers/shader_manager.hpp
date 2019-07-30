#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <vvv3d/core/graphics/managers/base_manager.hpp>
#include <vvv3d/core/graphics/shaders/shader.hpp>

namespace vvv3d {
class ShaderManager : public BaseManager<Shader> {
public:
    ShaderManager();

    void addFromFiles(const std::string& name,
                      const std::string& vertexShaderFilename,
                      const std::string& fragmentShaderFilename);
    void addFromFiles(const std::string& name,
                      const std::string& vertexShaderFilename,
                      const std::string& fragmentShaderFilename,
                      const std::string& geometryShaderFilename);
    void addFromSource(const std::string& name,
                       const std::string& vertexShaderSource,
                       const std::string& fragmentShaderSource);
    void addFromSource(const std::string& name,
                       const std::string& vertexShaderSource,
                       const std::string& fragmentShaderSource,
                       const std::string& geometryShaderSource);
};
} // namespace vvv3d
