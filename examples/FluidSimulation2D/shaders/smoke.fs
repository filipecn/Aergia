#version 150 core

in vec2 texCoord;
out float outColor;

uniform sampler2D d;
uniform sampler2D s;

uniform float c;

void main() {
    outColor = clamp(texture(d, texCoord).r + c*texture(s, texCoord).r, 0.0, 1.0);
}