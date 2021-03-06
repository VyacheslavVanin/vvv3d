#include "assert.h"
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

namespace {
template <typename T>
void beginDrawToTextures_impl(
    const std::vector<std::reference_wrapper<T>>& color_textures)
{
    const GLsizei color_textures_count = color_textures.size();
    for (GLsizei i = 0; i < color_textures_count; ++i) {
        const auto& texture_ref = color_textures[i];
        const auto& id = texture_ref.get().getID();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
                               GL_TEXTURE_2D, id, 0);
    }

    static const GLenum DrawBuffers[] = {
        GL_COLOR_ATTACHMENT0,      GL_COLOR_ATTACHMENT0 + 1,
        GL_COLOR_ATTACHMENT0 + 2,  GL_COLOR_ATTACHMENT0 + 3,
        GL_COLOR_ATTACHMENT0 + 4,  GL_COLOR_ATTACHMENT0 + 5,
        GL_COLOR_ATTACHMENT0 + 6,  GL_COLOR_ATTACHMENT0 + 7,
        GL_COLOR_ATTACHMENT0 + 8,  GL_COLOR_ATTACHMENT0 + 9,
        GL_COLOR_ATTACHMENT0 + 10, GL_COLOR_ATTACHMENT0 + 11,
        GL_COLOR_ATTACHMENT0 + 12, GL_COLOR_ATTACHMENT0 + 13,
        GL_COLOR_ATTACHMENT0 + 14, GL_COLOR_ATTACHMENT0 + 15,
        GL_COLOR_ATTACHMENT0 + 16, GL_COLOR_ATTACHMENT0 + 17,
        GL_COLOR_ATTACHMENT0 + 18, GL_COLOR_ATTACHMENT0 + 19,
        GL_COLOR_ATTACHMENT0 + 20, GL_COLOR_ATTACHMENT0 + 21,
        GL_COLOR_ATTACHMENT0 + 22, GL_COLOR_ATTACHMENT0 + 23,
        GL_COLOR_ATTACHMENT0 + 24, GL_COLOR_ATTACHMENT0 + 25,
        GL_COLOR_ATTACHMENT0 + 26, GL_COLOR_ATTACHMENT0 + 27,
        GL_COLOR_ATTACHMENT0 + 28, GL_COLOR_ATTACHMENT0 + 29,
        GL_COLOR_ATTACHMENT0 + 30, GL_COLOR_ATTACHMENT0 + 31,
    };
    glDrawBuffers(color_textures_count, DrawBuffers);
}

template <typename T>
void beginDrawToTextures(
    GLuint framebuffer,
    const std::vector<std::reference_wrapper<T>>& color_textures,
    T& depthTexture)
{
    assert(depthTexture.getFormat() == GL_DEPTH_COMPONENT ||
           depthTexture.getFormat() == GL_DEPTH_COMPONENT32F);
    assert(color_textures.size() <= 32);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           depthTexture.getID(), 0);

    beginDrawToTextures_impl(color_textures);

    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

template <typename T>
void beginDrawToTextures(
    GLuint framebuffer,
    const std::vector<std::reference_wrapper<T>>& color_textures)
{
    assert(color_textures.size() <= 32);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    beginDrawToTextures_impl(color_textures);

    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}
} // namespace

bool FrameBufferObject::beginDrawToTextures(
    const std::vector<std::reference_wrapper<Texture>>& color_textures,
    Texture& depthTexture)
{
    ::beginDrawToTextures(framebuffer, color_textures, depthTexture);
    return false;
}

bool FrameBufferObject::beginDrawToTextures(
    const std::vector<std::reference_wrapper<LowLevelTexture>>& color_textures,
    LowLevelTexture& depthTexture)
{
    ::beginDrawToTextures(framebuffer, color_textures, depthTexture);
    return false;
}

bool FrameBufferObject::beginDrawToTextures(
    const std::vector<std::reference_wrapper<Texture>>& color_textures)
{
    ::beginDrawToTextures(framebuffer, color_textures);
    return false;
}
bool FrameBufferObject::beginDrawToTextures(
    const std::vector<std::reference_wrapper<LowLevelTexture>>& color_textures)
{
    ::beginDrawToTextures(framebuffer, color_textures);
    return false;
}

bool FrameBufferObject::beginDrawToTextures(LowLevelTexture& colorTexture,
                                            LowLevelTexture& depthTexture)
{
    assert(depthTexture.getFormat() == GL_DEPTH_COMPONENT ||
           depthTexture.getFormat() == GL_DEPTH_COMPONENT32F);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           colorTexture.getID(), 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           depthTexture.getID(), 0);

    static const GLenum DrawBuffers[] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers);

    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    return true;
}

bool FrameBufferObject::beginDrawToColorTexture(LowLevelTexture& colorTexture)
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           colorTexture.getID(), 0);

    static const GLenum DrawBuffers[] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers);

    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    return true;
}

bool FrameBufferObject::beginDrawToDepthTexture(LowLevelTexture& depth)
{
    assert(depth.getFormat() == GL_DEPTH_COMPONENT ||
           depth.getFormat() == GL_DEPTH_COMPONENT32F);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           depth.getID(), 0);

    glDrawBuffers(0, nullptr);

    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    return true;
}

void FrameBufferObject::endDraw() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

namespace vvv3d {
void drawToColorTexture(LowLevelTexture& texture,
                        const std::function<void()>& draw)
{
    FrameBufferObject fbo;
    fbo.beginDrawToColorTexture(texture);
    draw();
    fbo.endDraw();
}

void drawToDepthTexture(LowLevelTexture& texture,
                        const std::function<void()>& draw)
{
    FrameBufferObject fbo;
    fbo.beginDrawToDepthTexture(texture);
    draw();
    fbo.endDraw();
}

void drawToColorAndDepthTextures(LowLevelTexture& color_texture,
                                 LowLevelTexture& depth_texture,
                                 const std::function<void()>& draw)
{
    FrameBufferObject fbo;
    fbo.beginDrawToTextures(color_texture, depth_texture);
    draw();
    fbo.endDraw();
}

void drawToColorAndDepthTextures(
    const std::vector<std::reference_wrapper<Texture>>& color_textures,
    Texture& depth_texture, const std::function<void()>& draw)
{
    FrameBufferObject fbo;
    fbo.beginDrawToTextures(color_textures, depth_texture);
    draw();
    fbo.endDraw();
}

void drawToColorAndDepthTextures(
    const std::vector<std::reference_wrapper<LowLevelTexture>>& color_textures,
    LowLevelTexture& depth_texture, const std::function<void()>& draw)
{
    FrameBufferObject fbo;
    fbo.beginDrawToTextures(color_textures, depth_texture);
    draw();
    fbo.endDraw();
}

void drawToColorTextures(
    const std::vector<std::reference_wrapper<Texture>>& color_textures,
    const std::function<void()>& draw)
{
    FrameBufferObject fbo;
    fbo.beginDrawToTextures(color_textures);
    draw();
    fbo.endDraw();
}

void drawToColorTextures(
    const std::vector<std::reference_wrapper<LowLevelTexture>>& color_textures,
    const std::function<void()>& draw)
{
    FrameBufferObject fbo;
    fbo.beginDrawToTextures(color_textures);
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
    drawToColorAndDepthTextures(color_texture.getLowLevelTexture(),
                                depth_texture.getLowLevelTexture(), draw);
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
