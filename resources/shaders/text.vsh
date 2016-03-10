#version 330 core
in vec2 position;
in vec2 texCoord;
uniform mat4    modelMatrix;
uniform mat4    viewProjection;

out highp vec2  out_texCoord;

void main(void)
{
    gl_Position  = viewProjection*modelMatrix*vec4(position, 0 ,1) ;
    out_texCoord = texCoord;
}
