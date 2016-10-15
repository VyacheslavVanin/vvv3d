#version 330 core
in vec2 position;
in vec2 texCoord;
uniform mat4    modelMatrix;
uniform mat4    viewProjectionMatrix;


void main(void)
{
    gl_Position  = viewProjectionMatrix*modelMatrix*vec4( position, 0 ,1 ) ;
}
