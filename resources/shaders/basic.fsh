#version 330 core
precision mediump float;
uniform float time;
uniform sampler2D texture0;

in  vec3 out_normal;
in  vec3 out_position;
in  vec2 out_texCoord;
out vec4 color;

vec3 lightPos = vec3(10, 10, 10);

void main(void)
{
    float lightIntensity = 100;
    vec3 toLight = lightPos - out_position;
    lightIntensity *= dot( normalize( toLight ), out_normal);
    lightIntensity /= dot( toLight, toLight );
    vec4 diffuseColor = texture2D( texture0, out_texCoord );
    vec3 colorComponent = diffuseColor.rgb;
    color = vec4( lightIntensity*colorComponent, diffuseColor.w);
}
