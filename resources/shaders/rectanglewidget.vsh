#version 330 core
in vec2 position;
uniform mat4    modelMatrix;
uniform mat4    viewProjection;


void main(void)
{
    gl_Position  = viewProjection*modelMatrix*vec4( position, 0 ,1 ) ;
}
