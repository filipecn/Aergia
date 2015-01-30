#version 150 core

in vec2 texCoord;
out float outColor;

struct grid {
    vec2 offset;
    sampler2D m;
};

uniform grid u;
uniform grid v;
uniform grid q;
uniform float dx;
uniform float dt;

vec2 worldCoordinates(grid g, vec2 c){
    return c*dx + g.offset;
}

vec2 textureCoordinates(grid g, vec2 w){
    return (w - g.offset)/dx;
}

void main() {
    vec2 w = worldCoordinates(q, texCoord);
    vec2 pos = w - dt * dx * vec2(texture(u.m, texCoord).r,texture(v.m, texCoord).r);

    outColor = texture(q.m, textureCoordinates(q, pos)).r;
}