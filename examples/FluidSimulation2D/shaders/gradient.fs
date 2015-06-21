#version 150 core

in vec2 texCoord;
out float outColor;

struct grid {
    vec2 offset;
    vec2 size;
    sampler2D m;
};

uniform grid p;
uniform grid c;

uniform float s;
uniform vec2 delta;

vec2 uvcoords(grid g, ivec2 iCoord){
    return (iCoord + vec2(0.5))/g.size;
}

void main() {
    ivec2 coord = ivec2(texCoord.s * c.size.x, texCoord.t * c.size.y);

    ivec2 ccoord = ivec2(coord - delta);

    outColor = texture(c.m, texCoord) - s*(
       texture(p.m, uvcoords(p, coord) + vec2(c.offset.x/p.size.x,c.offset.y/p.size.y)).r -
       texture(p.m, uvcoords(p, ccoord)+ vec2(c.offset.x/p.size.x,c.offset.y/p.size.y)).r);
}