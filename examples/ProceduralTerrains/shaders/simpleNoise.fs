#version 150 core

in vec3 texCoord;
out float outColor;

in float gLayer;

void main()
{
    ivec3 voxelCoord = ivec3(gl_FragCoord.xy, gLayer);
    //voxelOperation(voxelCoord);
    outColor = gLayer;
}