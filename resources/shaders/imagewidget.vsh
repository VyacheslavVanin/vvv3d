#version 330 core
in vec2 va_position;
in vec2 va_texCoord;
uniform mat4    viewProjectionMatrix;
uniform vec4    texturePosition;
uniform vec4    position;
out highp vec2  out_texCoord;


void main(void)
{
    vec2 fullPosition = position.xy + va_position*position.zw;
    gl_Position  = viewProjectionMatrix*vec4(fullPosition, 0 ,1 ) ;
    out_texCoord = texturePosition.xy + va_texCoord*texturePosition.zw;
}
