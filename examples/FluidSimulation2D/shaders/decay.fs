#version 440 core

in vec2 texCoord;
out float outColor;

uniform sampler2D q;

uniform float c;

void main() {
    outColor = texture(q, texCoord).r*c;
}