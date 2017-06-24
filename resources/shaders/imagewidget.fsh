#version 330 core
precision mediump float;
uniform sampler2D texture0;
uniform float time;

in  vec2 out_texCoord;
out vec4 color;


void main(void)
{
    color = texture2D( texture0, out_texCoord );
}
