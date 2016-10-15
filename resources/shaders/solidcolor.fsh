#version 330 core
precision mediump float;
uniform float time;
uniform vec4 colour0;

out vec4 color;


void main(void)
{
    color = colour0;
}
