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

ivec2 icoords(grid g, vec2 tCoord){
    return ivec2(tCoord.s * g.size.x, tCoord.t * g.size.y);
}

vec2 uvcoords(grid g, ivec2 iCoord){
    return vec2((iCoord.x + 0.5)/g.size.x, (iCoord.y + 0.5)/g.size.y);
}

void main() {
    ivec2 coord = icoords(b, texCoord);

    ivec2 xL = ivec2(coord - ivec2(1,0));
    ivec2 xR = ivec2(coord + ivec2(1,0));
    ivec2 xB = ivec2(coord - ivec2(0,1));
    ivec2 xT = ivec2(coord + ivec2(0,1));

    outColor = (    texture(x.m, uvcoords(x,xL)).r +
                    texture(x.m, uvcoords(x,xR)).r +
                    texture(x.m, uvcoords(x,xB)).r +
                    texture(x.m, uvcoords(x,xT)).r +
                    alpha * texture(b.m, texCoord).r ) * beta;
}