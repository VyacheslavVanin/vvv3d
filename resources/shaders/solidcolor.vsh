#version 330 core
in vec2 va_position;
uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;


void main(void)
{
    gl_Position  = viewProjectionMatrix*modelMatrix*vec4(va_position, 0 ,1 ) ;
}
