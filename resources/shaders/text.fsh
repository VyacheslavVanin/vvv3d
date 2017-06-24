#version 330 core
precision mediump float;
uniform sampler2D texture0;
uniform vec4 color0;

in  vec2 out_texCoord;
out vec4 color;


void main(void)
{
    color = color0*texture2D(texture0, out_texCoord);
}
