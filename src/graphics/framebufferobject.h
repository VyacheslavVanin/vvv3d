#ifndef FRAMEBUFFEROBJECT_H
#define FRAMEBUFFEROBJECT_H
#include "textures/texture.h"

class FrameBufferObject
{
public:
    FrameBufferObject();

    ~FrameBufferObject();
    FrameBufferObject(const FrameBufferObject& other) = delete;
    FrameBufferObject& operator=(const FrameBufferObject& other) = delete;

/* Example:
   Texture( std::shared_ptr<LowLevelTexture>( new LowLevelTexture(0,1024,1024,GL_RGBA,GL_RGBA,GL_UNSIGNED_BYTE)) ));
   Texture( std::shared_ptr<LowLevelTexture>( new LowLevelTexture(0,1024,1024,GL_DEPTH_COMPONENT,GL_DEPTH_COMPONENT,GL_FLOAT)) )); */
    bool beginDrawToTextures( Texture& colourTexture, Texture& depthTexture);
    bool beginDrawToColourTexture(Texture& colourTexture);
    bool beginDrawToDepthTexture(Texture& depth);

    void endDraw();

private:
    GLuint framebuffer;
};

#endif // FRAMEBUFFEROBJECT_H

