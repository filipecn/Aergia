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
    return (uv*g.size - vec2(0.5) + g.offset)*vec2(dx);
}

vec2 w_uv(grid g, vec2 w){
    return (w - g.offset*vec2(dx) + vec2(dx*0.5))/(g.size*vec2(dx));
}

void main() {
    vec2 w = uv_w(q, texCoord);
    vec2 pos = w - dt*dx*vec2(texture(u.m, w_uv(u, w)).r,texture(v.m, w_uv(v, w)).r);
    outColor = texture(q.m, w_uv(q, pos)).r;
}