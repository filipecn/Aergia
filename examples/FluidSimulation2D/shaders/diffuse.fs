#version 440 core

in vec2 texCoord;
out float outColor;

uniform sampler2D q;
uniform sampler2D cellType;

uniform float s;

void main() {
    ivec2 texel = ivec2(gl_FragCoord.xy);

    float Q = texelFetchOffset(q, texel, 0, ivec2(0, 0)).r;
    if(texelFetchOffset(cellType, texel, 0, ivec2(0,0)).r > 0.0){
            outColor = Q;
            return;
    }

    float E = texelFetchOffset(q, texel, 0, ivec2(1, 0)).r;
    if(texelFetchOffset(cellType, texel, 0, ivec2(1,0)).r > 0.0)
        E = 273.0;

    float W = texelFetchOffset(q, texel, 0, ivec2(-1,0)).r;
    if(texelFetchOffset(cellType, texel, 0, ivec2(-1,0)).r > 0.0)
        W = 273.0;

    float N = texelFetchOffset(q, texel, 0, ivec2(0,1)).r;
    if(texelFetchOffset(cellType, texel, 0, ivec2(0,1)).r > 0.0)
        N = 273.0;

    float S = texelFetchOffset(q, texel, 0, ivec2(0,-1)).r;
    if(texelFetchOffset(cellType, texel, 0, ivec2(0,-1)).r > 0.0)
        S = 273.0;

    outColor = Q + s*( E + W + N + S - 4.0*Q );
}