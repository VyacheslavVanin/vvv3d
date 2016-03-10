#include "framebufferobject.h"

FrameBufferObject::FrameBufferObject()
{
    glGenFramebuffers(1,&framebuffer);
}

FrameBufferObject::~FrameBufferObject()
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glDeleteFramebuffers(1,&framebuffer);
}

bool FrameBufferObject::beginDrawToTextures(Texture& colorTexture,
                                            Texture& depthTexture)
{
    if(depthTexture.getFormat() != GL_DEPTH_COMPONENT)
        throw std::logic_error("FrameBufferObject depth texture must have GL_DEPTH_COMPONENT internal format");

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture.getID(), 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture.getID(), 0);

    static const GLenum DrawBuffers[] = {GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT};
    glDrawBuffers(2,DrawBuffers);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        endDraw();
        throw std::logic_error("FrameBufferObject incomplete!!! code"); }
    return true;
}




bool FrameBufferObject::beginDrawToDepthTexture(Texture& depthTexture)
{
    if(depthTexture.getFormat()!=GL_DEPTH_COMPONENT)
        throw std::logic_error("FrameBufferObject depth texture must have GL_DEPTH_COMPONENT internal format");

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture.getID(), 0);

    static const GLenum DrawBuffers[] = {GL_DEPTH_ATTACHMENT};
    glDrawBuffers( 1, DrawBuffers);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        endDraw();
        throw std::logic_error("FrameBufferObject incomplete!!! code"); }
    return true;
}

void FrameBufferObject::endDraw()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


bool FrameBufferObject::beginDrawToColourTexture(Texture &colorTexture)
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,colorTexture.getID(),0);

    static const GLenum DrawBuffers[] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1,DrawBuffers);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        endDraw();
        throw std::logic_error("FrameBufferObject incomplete!!! code"); }
    return true;
}
