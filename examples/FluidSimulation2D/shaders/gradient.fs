#version 150 core

in vec2 texCoord;
out float outColor;

struct grid {
    vec2 size;
    sampler2D m;
};

uniform grid p;
uniform grid c;

uniform float s;
uniform vec2 delta;

vec2 uvCoord(ivec2 coord, grid g){
    float S = (coord.x/g.size.x) + (1.0/(2.0*g.size.x));
    float T = (coord.y/g.size.y) + (1.0/(2.0*g.size.y));
    return vec2(S,T);
}

void main() {
    ivec2 coord = ivec2(texCoord.s * c.size.x, texCoord.t * c.size.y);

    ivec2 ccoord = ivec2(coord - delta);

    outColor = texture(c.m, texCoord) - s*(
                                            texture(p.m, uvCoord(coord, p)).r -
                                  texture(p.m, uvCoord(ccoord, p)).r);
}