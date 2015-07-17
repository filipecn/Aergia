#version 440 core

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
uniform grid cellType;
uniform float dx;
uniform float dt;

vec2 uv_w(grid g, vec2 uv){
    return (uv*g.size - vec2(0.5) + g.offset)*vec2(dx);
}

vec2 w_uv(grid g, vec2 w){
    return (w - g.offset*vec2(dx) + vec2(dx*0.5))/(g.size*vec2(dx));
}

ivec2 uv_grid(grid g, vec2 uv){
    return ivec2(int(uv/(1.0/g.size)));
}

ivec2 w_grid(grid g, vec2 w){
    return uv_grid(g, w_uv(g, w));
}

float getValue(grid g, vec2 wstart, vec2 wend){
    // check if end is solid
    if(texture(cellType.m, w_uv(cellType, wend)).r > 0.0){
        return 0.0;
    }
    return texture(g.m, w_uv(g, wend)).r;
}

void main() {
    // start position
    vec2 wstart = uv_w(q, texCoord);
    // final position
    vec2 wmid = wstart - 0.5*dt*vec2(texture(u.m, w_uv(u, wstart)).r,texture(v.m, w_uv(v, wstart)).r);
    vec2 wend = wstart - dt*vec2(texture(u.m, w_uv(u, wmid)).r,texture(v.m, w_uv(v, wmid)).r);
    wend = wstart - dt*vec2(texture(u.m, w_uv(u, wstart)).r,texture(v.m, w_uv(v, wstart)).r);
    outColor = getValue(q, wstart, wend);
}