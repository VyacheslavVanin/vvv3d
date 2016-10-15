#version 330 core
uniform mat4    modelMatrix;
uniform mat4    viewProjectionMatrix;

in vec2 va_position;
in vec2 va_texCoord;
out highp vec2  out_texCoord;

void main(void)
{
    gl_Position  = viewProjectionMatrix*modelMatrix*vec4(va_position, 0 ,1) ;
    out_texCoord = va_texCoord;
}
