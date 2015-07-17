#version 440 core

in vec2 texCoord;
out float outColor;

struct grid {
    vec2 size;
    sampler2D m;
};

uniform grid u;
uniform grid v;
uniform grid cellType;

uniform float s;

vec2 uvcoords(grid g, ivec2 iCoord){
    return (iCoord + vec2(0.5))/g.size;
}

void main() {
    //integer coordinates
    ivec2 coord = ivec2(texCoord.s * v.size.x, texCoord.t * u.size.y);

    ivec2 ucoord = ivec2(coord + ivec2(1,0));
    ivec2 vcoord = ivec2(coord + ivec2(0,1));

    outColor = -s*( texture(u.m, uvcoords(u, ucoord)).r -
                    texture(u.m, uvcoords(u,  coord)).r +
                    texture(v.m, uvcoords(v, vcoord)).r -
                    texture(v.m, uvcoords(v,  coord)).r );
    //outColor = texture(u.m, uvcoords(v, ucoord)).r;
    ivec2 texel = ivec2(gl_FragCoord.xy);

    float UE = texelFetchOffset(u.m, texel, 0, ivec2(1, 0)).r;
    if(texelFetchOffset(cellType.m, texel, 0, ivec2(1,0)).r > 0.0)
        UE = 0.0;

    float UW = texelFetchOffset(u.m, texel, 0, ivec2(0, 0)).r;
    if(texelFetchOffset(cellType.m, texel, 0, ivec2(-1,0)).r > 0.0)
        UW = 0.0;

    float VN = texelFetchOffset(v.m, texel, 0, ivec2(0, 1)).r;
    if(texelFetchOffset(cellType.m, texel, 0, ivec2(0,1)).r > 0.0)
        VN = 0.0;

    float VS = texelFetchOffset(v.m, texel, 0, ivec2(0, 0)).r;
    if(texelFetchOffset(cellType.m, texel, 0, ivec2(0,-1)).r > 0.0)
        VS = 0.0;
        
    outColor = -s*( UE - UW + VN - VS );
}