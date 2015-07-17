#version 440 core

in vec2 texCoord;
out float outColor;

uniform sampler2D d;
uniform sampler2D s;

uniform float c;
uniform vec2 mouse;
uniform float globalTime;

void main() {
    vec2 flameCenter = mouse;
    vec2 dis = flameCenter - texCoord;
    float smoke = texture(s, texCoord).r;

    if(length(dis) < 0.02 + 0.008*cos(globalTime*200.0)){
        float _c = 0.1;
        float a = 1.0/(sqrt(6.28));
        float f = 2.0*a*exp(-dot(dis,dis)/(2.0*_c*_c));
        smoke = f;// + 0.5*cos(globalTime*500.0);
    }

    outColor = clamp(texture(d, texCoord).r + c*smoke, 0.0, 1.0);
}