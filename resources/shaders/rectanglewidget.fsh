#version 330 core
precision mediump float;
in  vec2 out_position;
out vec4 colour;

uniform vec4 colour0;


void main(void)
{
    colour = colour0;
}
