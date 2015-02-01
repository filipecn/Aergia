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

void main() {
    ivec2 coord = ivec2(texCoord.s * v.size.x, texCoord.t * u.size.y);

    ivec2 ucoord = ivec2(coord + ivec2(1,0));
    ivec2 vcoord = ivec2(coord + ivec2(0,1));

    float uw = v.size.x;
    float uh = u.size.y - 1.0;
    float vw = v.size.x - 1.0;
    float vh = u.size.y;

    outColor = -s*( texture(u.m, vec2(ucoord.x/uw,ucoord.y/uh)).r -
                    texture(u.m, vec2( coord.x/uw, coord.y/uh)).r +
                    texture(v.m, vec2(vcoord.x/vw,vcoord.y/vh)).r -
                    texture(v.m, vec2( coord.x/vw, coord.y/vh)).r);

     outColor = texture(u.m, vec2(ucoord.x/uw,ucoord.y/uh)).r;
}