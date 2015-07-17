#version 440 core

in vec2 texCoord;
out vec4 outColor;

uniform sampler2D S;
uniform sampler2D T;
uniform sampler2D H;

uniform float maxTemp;
uniform int mode;
uniform float globalTime;
uniform vec2 mouse;

void main() {
    float s = texture(S, texCoord).r*1.3;
    float t = texture(T, texCoord).r;
    float h = texture(H, texCoord).r;

    if(mode == 2){
        float nh = (h - 273.0)/(maxTemp - 273.0);
        outColor = vec4(nh,0.0,0.0,1.0);
        return;
    }
    float nt = (t - 273.0)/(maxTemp - 273.0);
    if(mode == 1){
         outColor = vec4(nt,nt,0.0,1.0);
         return;
    }

    vec3 a = vec3(1.0,0.0,0.0);
    vec3 b = vec3(0.2,0.4,0.5);
    vec3 c = vec3(0.5,3.0,3.5);
    vec3 d = vec3(1.,0.4,0.4);
    vec3 fire = a + b*cos(2.0*3.141516*(c*nt + d));

    outColor = vec4(mix(vec3(s), fire*nt, 0.3), 1.0);

    vec2 flameCenter = mouse;
    vec2 _d = flameCenter - texCoord;
    float _c = 0.01 + 0.001*cos(globalTime*500.0);
    float _a = 4.0/(sqrt(6.28));
    float f = 0.8*_a*exp(-dot(_d,_d)/(2.0*_c*_c));
    outColor += vec4(vec2(f),0.0,1.0);

}