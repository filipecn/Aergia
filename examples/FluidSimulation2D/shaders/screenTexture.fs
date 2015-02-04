#version 150 core

in vec2 texCoord;
out float outColor;

uniform sampler2D tex;
void main() {
    outColor = texture(tex, texCoord).r;
}