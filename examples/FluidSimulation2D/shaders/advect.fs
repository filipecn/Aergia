#version 150 core

in vec2 texCoord;
out vec4 outColor;

uniform sampler2D tex;
void main() {
    outColor = texture(tex, texCoord) + vec4(0.001,0,0,0);
}