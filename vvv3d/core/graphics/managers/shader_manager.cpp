#include "shader_manager.hpp"

namespace vvv3d {

ShaderManager::ShaderManager() : BaseManager<Shader>() {}

void ShaderManager::addFromFiles(const std::string& name,
                                 const std::string& vertexShaderFilename,
                                 const std::string& fragmentShaderFilename)
{
    add(name, Shader::fromFiles(name.c_str(), vertexShaderFilename.c_str(),
                                fragmentShaderFilename.c_str()));
}

void ShaderManager::addFromFiles(const std::string& name,
                                 const std::string& vertexShaderFilename,
                                 const std::string& fragmentShaderFilename,
                                 const std::string& geometryShaderFilename)
{
    add(name, Shader::fromFiles(name.c_str(), vertexShaderFilename.c_str(),
                                fragmentShaderFilename.c_str(),
                                geometryShaderFilename.c_str()));
}

void ShaderManager::addFromSource(const std::string& name,
                                  const std::string& vertexShaderSource,
                                  const std::string& fragmentShaderSource)
{
    add(name, Shader::fromStrings(name.c_str(), vertexShaderSource.c_str(),
                                  fragmentShaderSource.c_str()));
}

void ShaderManager::addFromSource(const std::string& name,
                                  const std::string& vertexShaderSource,
                                  const std::string& fragmentShaderSource,
                                  const std::string& geometryShaderSource)
{
    add(name, Shader::fromStrings(name.c_str(), vertexShaderSource.c_str(),
                                  fragmentShaderSource.c_str(),
                                  geometryShaderSource.c_str()));
}

} // namespace vvv3d
