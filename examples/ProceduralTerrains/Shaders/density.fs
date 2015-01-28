#version 140
flat in int instanceID;

out vec4 outColor;

void main()
{
    ivec3 voxelCoord = ivec3(gl_FragCoord.xy, instanceID);
    //voxelOperation(voxelCoord);
    outColor = vec4(0,0,0,0);
}