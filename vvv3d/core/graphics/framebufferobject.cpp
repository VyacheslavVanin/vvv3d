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
