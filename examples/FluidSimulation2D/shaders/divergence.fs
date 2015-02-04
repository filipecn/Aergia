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

vec2 uvCoord(ivec2 coord, grid g){
    float S = (coord.x/g.size.x) + (1.0/(2.0*g.size.x));
    float T = (coord.y/g.size.y) + (1.0/(2.0*g.size.y));
    return vec2(S,T);
}

void main() {
    ivec2 coord = ivec2(texCoord.s * v.size.x, texCoord.t * u.size.y);

    ivec2 ucoord = ivec2(coord + ivec2(1,0));
    ivec2 vcoord = ivec2(coord + ivec2(0,1));

    outColor = -s*( texture(u.m, uvCoord(ucoord, u)).r -
                    texture(u.m, uvCoord( coord, u)).r +
                    texture(v.m, uvCoord(vcoord, v)).r -
                    texture(v.m, uvCoord( coord, v)).r );
}