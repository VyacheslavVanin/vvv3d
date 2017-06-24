#version 330 core
in vec2 va_position;
in vec2 va_texCoord;
uniform mat4    modelMatrix;
uniform mat4    viewProjectionMatrix;
uniform vec4    texturePosition;
out highp vec2  out_texCoord;


void main(void)
{
    gl_Position  = viewProjectionMatrix*modelMatrix*vec4(va_position, 0 ,1 ) ;
    out_texCoord = texturePosition.xy + va_texCoord*texturePosition.zw;
}
