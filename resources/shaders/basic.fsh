#version 330 core
precision mediump float;
in  vec3 out_normal;
in  vec3 out_position;
in  vec2 out_texCoord;
uniform float time;
uniform sampler2D texture0;

vec3 lightPos = vec3(10, 10, 10);
out vec4 color;


void main(void)
{
    //gl_FragColor = out_color;// * (0.5 - 0.5*sin(time));
    float lightIntensity = 100;
                      vec3 toLight = lightPos - out_position;
    lightIntensity *= dot( normalize( toLight ), out_normal);
    lightIntensity /= dot( toLight, toLight );
    vec4 diffuseColor = texture2D( texture0, out_texCoord );
    vec3 colorComponent = diffuseColor.rgb;
    color = vec4( lightIntensity*colorComponent, diffuseColor.w);
    //color = vec4(1,1,1,1)*lightIntensity;
}
