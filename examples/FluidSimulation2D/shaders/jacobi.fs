#version 440 core

in vec2 texCoord;
out float outColor;

struct grid {
    vec2 size;
    sampler2D m;
};

uniform grid x;
uniform grid b;
uniform grid cellType;
uniform float alpha;
uniform float beta;

ivec2 icoords(grid g, vec2 tCoord){
    return ivec2(tCoord.s * g.size.x, tCoord.t * g.size.y);
}

vec2 uvcoords(grid g, ivec2 iCoord){
    return (iCoord + vec2(0.5))/g.size;
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
    outColor = (    texture(x.m, uvcoords(x,xL)).r +
                                   texture(x.m, uvcoords(x,xR)).r +
                                   texture(x.m, uvcoords(x,xB)).r +
                                   texture(x.m, uvcoords(x,xT)).r +
                                   texture(b.m, texCoord).r ) * beta;

    ivec2 texel = ivec2(gl_FragCoord.xy);

    float PC = texelFetchOffset(x.m, texel, 0, ivec2(0, 0)).r;

    float PE = texelFetchOffset(x.m, texel, 0, ivec2(1, 0)).r;
    if(texelFetchOffset(cellType.m, texel, 0, ivec2(1,0)).r > 0.0){
        PE = PC;
    }
    float PW = texelFetchOffset(x.m, texel, 0, ivec2(-1, 0)).r;
    if(texelFetchOffset(cellType.m, texel, 0, ivec2(-1,0)).r > 0.0){
        PW = PC;
    }
    float PN = texelFetchOffset(x.m, texel, 0, ivec2(0, 1)).r;
    if(texelFetchOffset(cellType.m, texel, 0, ivec2(0,1)).r > 0.0){
        PN = PC;
    }
    float PS = texelFetchOffset(x.m, texel, 0, ivec2(0, -1)).r;
    if(texelFetchOffset(cellType.m, texel, 0, ivec2(0,-1)).r > 0.0){
        PS = PC;
    }

    outColor = (PE + PW + PN + PS + texture(b.m, texCoord).r) * beta;

}