#version 150 core

in vec2 texCoord;
out vec4 outColor;

uniform sampler2D S;
uniform sampler2D T;
uniform sampler2D H;

uniform float maxTemp;

void main() {
    float s = texture(S, texCoord).r*1.3;
    float t = texture(T, texCoord).r;
    float h = texture(H, texCoord).r;
    float nt = (t - 273.0)/(maxTemp - 273.0);

    vec3 a = vec3(1.0,1.0,1.0);
    vec3 b = vec3(0.2,0.4,0.5);
    vec3 c = vec3(0.5,3.0,3.5);
    vec3 d = vec3(1.,0.4,0.4);
    vec3 fire = a + b*cos(2.0*3.141516*(c*nt + d));

    outColor = vec4(mix(vec3(s), fire*nt, 0.3), 1.0);
}