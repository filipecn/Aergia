#version 150 core

in vec3 texCoord;
out vec4 outColor;

uniform sampler3D tex;
void main() {
    outColor = texture3D(tex, texCoord);
}