#version 150 core

in vec2 texCoord;
out float outColor;

struct grid {
    vec2 offset;
    vec2 size;
    sampler2D m;
};

uniform grid q;
uniform grid t;
uniform grid s;

ivec2 icoords(grid g, vec2 tCoord){
    return ivec2(tCoord.s * g.size.x, tCoord.t * g.size.y);
}

vec2 uvcoords(grid g, ivec2 iCoord){
    return vec2((iCoord.x + 0.5)/g.size.x, (iCoord.y + 0.5)/g.size.y);
}

uniform float dt;
uniform float f;
uniform float alpha;
uniform float beta;
uniform float Tamb;

void main() {

    ivec2 coord = icoords(q, texCoord);
    vec2 uv = uvcoords(t, coord);
    uv +=  vec2(q.offset.x/t.size.x,q.offset.y/t.size.y);

    outColor = texture(q.m, texCoord).r +
                    dt*(alpha*texture(s.m,uv).r - beta*(texture(t.m,uv).r - Tamb))*f;
}