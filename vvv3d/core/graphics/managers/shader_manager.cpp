#include "shader_manager.hpp"
#include "utils/helper.hpp"

namespace vvv3d {
using vvv::helper::format;

ShaderManager::ShaderManager() : shaders() {}

void ShaderManager::add(const std::string& name,
                        const std::string& vertexShaderFilename,
                        const std::string& fragmentShaderFilename)
{
    if (contain(name))
        return;

    shaders[name] =
        Shader::fromFiles(name.c_str(), vertexShaderFilename.c_str(),
                          fragmentShaderFilename.c_str());
}

void ShaderManager::add(const std::string& name,
                        const std::string& vertexShaderFilename,
                        const std::string& fragmentShaderFilename,
                        const std::string& geometryShaderFilename)
{
    if (contain(name))
        return;

    shaders[name] = Shader::fromFiles(
        name.c_str(), vertexShaderFilename.c_str(),
        fragmentShaderFilename.c_str(), geometryShaderFilename.c_str());
}

void ShaderManager::addFromSource(const std::string& name,
                                  const std::string& vertexShaderSource,
                                  const std::string& fragmentShaderSource)
{
    if (contain(name))
        return;

    shaders[name] = Shader::fromStrings(
        name.c_str(), vertexShaderSource.c_str(), fragmentShaderSource.c_str());
}

void ShaderManager::addFromSource(const std::string& name,
                                  const std::string& vertexShaderSource,
                                  const std::string& fragmentShaderSource,
                                  const std::string& geometryShaderSource)
{
    if (contain(name))
        return;

    shaders[name] = Shader::fromStrings(
        name.c_str(), vertexShaderSource.c_str(), fragmentShaderSource.c_str(),
        geometryShaderSource.c_str());
}

void ShaderManager::add(const std::string& name, std::unique_ptr<Shader> shader)
{
    if (contain(name))
        return;

    shaders[name].swap(shader);
}

void ShaderManager::add(const std::string& name,
                        const std::function<std::unique_ptr<Shader>()>& f)
{
    initializers[name] = f;
}

Shader& ShaderManager::get(const std::string& name) const
try {
    return *shaders.at(name);
}
catch (std::exception& e) {
    LOG_ERROR(format("failed to get shader \"@\"", name));
    throw;
}

Shader& ShaderManager::get(const std::string& name)
try {
    if (!contain(name)) {
        auto it = initializers.find(name);
        if (it != initializers.end()) {
            LOG_ERROR(format("lazy initialized shader \"@\"", name));
            add(name, it->second());
        }
    }
    return *shaders.at(name);
}
catch (std::exception& e) {
    LOG_ERROR(format("failed to get shader \"@\"", name));
    throw;
}

bool ShaderManager::contain(const std::string& name) const
{
    return shaders.find(name) != shaders.end();
}

std::vector<std::string> ShaderManager::listNames() const
{
    std::vector<std::string> ret;
    for (const auto& kv : shaders)
        ret.push_back(kv.first);
    return ret;
}
} // namespace vvv3d
