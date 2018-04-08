#include "framebufferobject.hpp"

using namespace vvv3d;

FrameBufferObject::FrameBufferObject() : framebuffer(0)
{
    glGenFramebuffers(1, &framebuffer);
}

FrameBufferObject::~FrameBufferObject()
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glDeleteFramebuffers(1, &framebuffer);
}

bool FrameBufferObject::beginDrawToTextures(Texture& colorTexture,
                                            Texture& depthTexture)
{
    return beginDrawToTextures(colorTexture.getLowLevelTexture(),
                               depthTexture.getLowLevelTexture());
}

bool FrameBufferObject::beginDrawToColorTexture(Texture& colorTexture)
{
    return beginDrawToColorTexture(colorTexture.getLowLevelTexture());
}

bool FrameBufferObject::beginDrawToDepthTexture(Texture& depthTexture)
{
    return beginDrawToDepthTexture(depthTexture.getLowLevelTexture());
}

bool FrameBufferObject::beginDrawToTextures(LowLevelTexture& colorTexture,
                                            LowLevelTexture& depthTexture)
{
    if (depthTexture.getFormat() != GL_DEPTH_COMPONENT)
        throw std::logic_error("FrameBufferObject depth texture must have "
                               "GL_DEPTH_COMPONENT internal format");

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           colorTexture.getID(), 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           depthTexture.getID(), 0);

    static const GLenum DrawBuffers[] = {GL_COLOR_ATTACHMENT0,
                                         GL_DEPTH_ATTACHMENT};
    glDrawBuffers(2, DrawBuffers);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        endDraw();
        throw std::logic_error("FrameBufferObject incomplete!!! code");
    }
    return true;
}

bool FrameBufferObject::beginDrawToColorTexture(LowLevelTexture& colorTexture)
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           colorTexture.getID(), 0);

    static const GLenum DrawBuffers[] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        endDraw();
        throw std::logic_error("FrameBufferObject incomplete!!! code");
    }
    return true;
}

bool FrameBufferObject::beginDrawToDepthTexture(LowLevelTexture& depth)
{
    if (depth.getFormat() != GL_DEPTH_COMPONENT)
        throw std::logic_error("FrameBufferObject depth texture must have "
                               "GL_DEPTH_COMPONENT internal format");

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           depth.getID(), 0);

    static const GLenum DrawBuffers[] = {GL_DEPTH_ATTACHMENT};
    glDrawBuffers(1, DrawBuffers);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        endDraw();
        throw std::logic_error("FrameBufferObject incomplete!!! code");
    }
    return true;
}

void FrameBufferObject::endDraw() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

namespace vvv3d {
void drawToColorTexture(LowLevelTexture& texture, const std::function<void()>& draw)
{
    FrameBufferObject fbo;
    fbo.beginDrawToColorTexture(texture);
    draw();
    fbo.endDraw();
}

void drawToDepthTexture(LowLevelTexture& texture, const std::function<void()>& draw)
{
    FrameBufferObject fbo;
    fbo.beginDrawToDepthTexture(texture);
    draw();
    fbo.endDraw();
}

void drawToColorAndDepthTextures(LowLevelTexture& color_texture, LowLevelTexture& depth_texture,
                                 const std::function<void()>& draw)
{
    FrameBufferObject fbo;
    fbo.beginDrawToTextures(color_texture, depth_texture);
    draw();
    fbo.endDraw();
}

void drawToColorTexture(Texture& texture, const std::function<void()>& draw)
{
    drawToColorTexture(texture.getLowLevelTexture(), draw);
}

void drawToDepthTexture(Texture& texture, const std::function<void()>& draw)
{
    drawToDepthTexture(texture.getLowLevelTexture(), draw);
}

void drawToColorAndDepthTextures(Texture& color_texture, Texture& depth_texture,
                                 const std::function<void()>& draw)
{
    drawToColorAndDepthTextures(color_texture.getLowLevelTexture(), depth_texture.getLowLevelTexture(), draw);
}

Texture drawToColorTexture(size_t width, size_t height,
                           const std::function<void()>& draw)
{
    Texture ret(std::make_shared<LowLevelTexture>(
        nullptr, width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE));
    drawToColorTexture(ret, draw);
    return ret;
}

Texture drawToDepthTexture(size_t width, size_t height,
                           const std::function<void()>& draw)
{
    Texture ret(std::make_shared<LowLevelTexture>(
        nullptr, width, height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT,
        GL_FLOAT));
    drawToDepthTexture(ret, draw);
    return ret;
}

std::unique_ptr<LowLevelTexture>
drawToColorLowLevelTexture(size_t width, size_t height,
                           const std::function<void()>& draw)
{
    auto ret = std::make_unique<LowLevelTexture>(
        nullptr, width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
    drawToColorTexture(*ret, draw);
    return ret;
}

std::unique_ptr<LowLevelTexture>
drawToDepthLowLevelTexture(size_t width, size_t height,
                           const std::function<void()>& draw)
{
    auto ret = std::make_unique<LowLevelTexture>(
        nullptr, width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
    drawToDepthTexture(*ret, draw);
    return ret;
}
} // namespace vvv3d
