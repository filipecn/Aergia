#version 150 core

in vec2 texCoord;
out float outColor;

uniform sampler2D q;
uniform float dt;
uniform float f;

void main() {
    outColor = texture(q, texCoord).r + dt*f;
}