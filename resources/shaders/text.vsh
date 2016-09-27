#version 330 core
uniform mat4    modelMatrix;
uniform mat4    viewProjectionMatrix;

in vec2 position;
in vec2 texCoord;
out highp vec2  out_texCoord;

void main(void)
{
    gl_Position  = viewProjectionMatrix*modelMatrix*vec4(position, 0 ,1) ;
    out_texCoord = texCoord;
}
