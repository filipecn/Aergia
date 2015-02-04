#version 150 core

in vec2 texCoord;
out float outColor;

uniform sampler2D t;
uniform sampler2D h;

uniform float s;
uniform float Tamb;

void main() {
    outColor = texture(t, texCoord).r + s*(texture(h, texCoord).r - texture(t, texCoord).r);
}