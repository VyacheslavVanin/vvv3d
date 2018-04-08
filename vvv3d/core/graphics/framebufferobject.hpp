#pragma once
#include "textures/texture.hpp"
#include <functional>

namespace vvv3d {

class FrameBufferObject {
public:
    FrameBufferObject();

    ~FrameBufferObject();
    FrameBufferObject(const FrameBufferObject& other) = delete;
    FrameBufferObject& operator=(const FrameBufferObject& other) = delete;

    /* Example:
       Texture( std::shared_ptr<LowLevelTexture>( new
       LowLevelTexture(0,1024,1024,GL_RGBA,GL_RGBA,GL_UNSIGNED_BYTE)) ));
       Texture( std::shared_ptr<LowLevelTexture>( new
       LowLevelTexture(0,1024,1024,GL_DEPTH_COMPONENT,GL_DEPTH_COMPONENT,GL_FLOAT))
       )); */
    bool beginDrawToTextures(Texture& colorTexture, Texture& depthTexture);
    bool beginDrawToColorTexture(Texture& colorTexture);
    bool beginDrawToDepthTexture(Texture& depth);

    bool beginDrawToTextures(LowLevelTexture& colorTexture,
                             LowLevelTexture& depthTexture);
    bool beginDrawToColorTexture(LowLevelTexture& colorTexture);
    bool beginDrawToDepthTexture(LowLevelTexture& depth);

    void endDraw();

private:
    GLuint framebuffer;
};

void drawToColorTexture(Texture& texture, const std::function<void()>& draw);
void drawToDepthTexture(Texture& texture, const std::function<void()>& draw);
void drawToColorAndDepthTextures(Texture& color_texture, Texture& depth_texture,
                                 const std::function<void()>& draw);

void drawToColorTexture(LowLevelTexture& texture, const std::function<void()>& draw);
void drawToDepthTexture(LowLevelTexture& texture, const std::function<void()>& draw);
void drawToColorAndDepthTextures(LowLevelTexture& color_texture, LowLevelTexture& depth_texture,
                                 const std::function<void()>& draw);

Texture drawToColorTexture(size_t width, size_t height,
                           const std::function<void()>& draw);
Texture drawToDepthTexture(size_t width, size_t height,
                           const std::function<void()>& draw);

std::unique_ptr<LowLevelTexture>
drawToColorLowLevelTexture(size_t width, size_t height,
                           const std::function<void()>& draw);
std::unique_ptr<LowLevelTexture>
drawToColorLowLevelTexture(size_t width, size_t height,
                           const std::function<void()>& draw);

} // namespace vvv3d
