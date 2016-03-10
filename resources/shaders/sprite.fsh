#version 330 core
precision mediump float;
in  vec2 out_position;
in  vec2 out_texCoord;
uniform sampler2D texture0;
uniform float time;

out vec4 color;


void main(void)
{
    color = texture2D( texture0, out_texCoord );
}
