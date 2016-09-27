#version 330 core
uniform mat4    mvp;
uniform mat3    rotation_matrix;

in vec3 position;
in vec3 normal;
in vec2 texCoord;
out highp vec3  out_normal;
out highp vec3  out_position;
out highp vec2  out_texCoord;


void main(void)
{
    gl_Position  = mvp*vec4( position, 1 ) ;
    out_position = gl_Position.xyz;
    out_normal   = rotation_matrix* normal;
    out_texCoord = texCoord;
}
