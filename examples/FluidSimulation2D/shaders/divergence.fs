#version 150 core

in vec2 texCoord;
out float outColor;

struct grid {
    vec2 size;
    sampler2D m;
};

uniform grid u;
uniform grid v;
uniform float s;

vec2 uvcoords(grid g, ivec2 iCoord){
    return vec2((iCoord.x + 0.5)/g.size.x, (iCoord.y + 0.5)/g.size.y);
}

void main() {
    ivec2 coord = ivec2(texCoord.s * v.size.x, texCoord.t * u.size.y);

    ivec2 ucoord = ivec2(coord + ivec2(1,0));
    ivec2 vcoord = ivec2(coord + ivec2(0,1));

    outColor = -s*( texture(u.m, uvcoords(u, ucoord)).r -
                    texture(u.m, uvcoords(u,  coord)).r +
                    texture(v.m, uvcoords(v, vcoord)).r -
                    texture(v.m, uvcoords(v,  coord)).r );
}