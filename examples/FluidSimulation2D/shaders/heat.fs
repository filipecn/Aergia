#version 440 core

in vec2 texCoord;
out float outColor;

uniform sampler2D t;
uniform sampler2D h;

uniform float s;
uniform float globalTime;
uniform vec2 mouse;

void main() {
    vec2 flameCenter = mouse;
    vec2 d = flameCenter - texCoord;
    float heat = texture(h, texCoord).r;

    if(length(d) < 0.03){
        float c = 0.1;
        float a = 1.0/(sqrt(6.28));
        heat = max(273.0, 1200.0*a*exp(-dot(d,d)/(2.0*c*c)));
    }
    outColor = texture(t, texCoord).r + s*(heat - texture(t, texCoord).r);
}