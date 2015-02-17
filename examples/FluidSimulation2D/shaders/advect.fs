#version 150 core

in vec2 texCoord;
out float outColor;

struct grid {
    vec2 offset;
    vec2 size;
    sampler2D m;
};

uniform grid u;
uniform grid v;
uniform grid q;
uniform float dx;
uniform float dt;

vec2 uv_w(grid g, vec2 uv){
     return vec2(uv.x*g.size.x*dx + dx*g.offset.x,uv.y*g.size.y*dx + dx*g.offset.y);
}

vec2 w_uv(grid g, vec2 w){
    return vec2((w.x - g.offset.x*dx)/(dx*g.size.x),(w.y - g.offset.y*dx)/(dx*g.size.y));
}

void main() {
    vec2 w = uv_w(q, texCoord);

    vec2 pos = w - dt*dx*vec2(texture(u.m, w_uv(u, w)).r,texture(v.m, w_uv(v, w)).r);
    outColor = texture(q.m, w_uv(q, pos)).r;
}