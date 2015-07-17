#version 440 core

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

uniform float dx;
uniform float dt;
uniform float f;
uniform float alpha;
uniform float beta;
uniform float Tamb;

vec2 uv_w(grid g, vec2 uv){
    return (uv*g.size - vec2(0.5) + g.offset)*vec2(dx);
}

vec2 w_uv(grid g, vec2 w){
    return (w - g.offset*vec2(dx) + vec2(dx*0.5))/(g.size*vec2(dx));
}

void main() {
    vec2 uv = uv_w(q, texCoord);
    outColor = texture(q.m, texCoord).r + 
        dt*(alpha*texture(s.m, w_uv(s, uv)).r - beta*(texture(t.m, w_uv(t, uv)).r - Tamb))*f;
}