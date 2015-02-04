#version 150 core

in vec2 texCoord;
out float outColor;

struct grid {
    vec2 size;
    sampler2D m;
};

uniform grid x;
uniform grid b;
uniform float alpha;
uniform float beta;

vec2 uvCoord(ivec2 coord, grid g){
    float S = (coord.x/g.size.x) + (1.0/(2.0*g.size.x));
    float T = (coord.y/g.size.y) + (1.0/(2.0*g.size.y));
    return vec2(S,T);
}

void main() {
    ivec2 coord = ivec2(texCoord.s * b.size.x, texCoord.t * b.size.y);

    ivec2 xL = ivec2(coord - ivec2(1,0));
    ivec2 xR = ivec2(coord + ivec2(1,0));
    ivec2 xB = ivec2(coord - ivec2(0,1));
    ivec2 xT = ivec2(coord + ivec2(0,1));

    outColor = (    texture(x.m, uvCoord(xL, x)).r +
                    texture(x.m, uvCoord(xR, x)).r +
                    texture(x.m, uvCoord(xB, x)).r +
                    texture(x.m, uvCoord(xT, x)).r +
                    alpha * texture(b.m, texCoord).r ) * beta;
}