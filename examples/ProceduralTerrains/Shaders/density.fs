#version 140
int instanceID;

void main()
{
    ivec3 voxelCoord = ivec3(gl_FragCoord.xy, instanceID);
    voxelOperation(voxelCoord);
}