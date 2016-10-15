#version 330 core
uniform mat4    mvp;
uniform mat3    rotation_matrix;

in vec3 va_position;
in vec3 va_normal;
in vec2 va_texCoord;
out highp vec3  out_normal;
out highp vec3  out_position;
out highp vec2  out_texCoord;


void main(void)
{
    gl_Position  = mvp*vec4(va_position, 1);
    out_position = gl_Position.xyz;
    out_normal   = rotation_matrix* va_normal;
    out_texCoord = va_texCoord;
}
