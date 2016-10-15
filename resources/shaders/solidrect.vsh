#version 330 core
in vec2 va_position;
uniform mat4    viewProjectionMatrix;
uniform vec4    position;


void main(void)
{
    vec2 fullPosition = position.xy + va_position*position.zw;
    gl_Position  = viewProjectionMatrix*vec4(fullPosition, 0 ,1 ) ;
}
