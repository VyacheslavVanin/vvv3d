#version 330 core
precision mediump float;
in  vec2 out_position;
in  vec2 out_texCoord;
uniform sampler2D texture0;
uniform vec4 colour0;

out vec4 color;


void main(void)
{
    color = colour0*texture2D(texture0, out_texCoord);
}
