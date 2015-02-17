#version 150 core

in vec3 texCoord;
out vec4 outColor;

flat in int instanceID;

uniform sampler3D tex;

void main()
{
    ivec3 voxelCoord = ivec3(gl_FragCoord.xy, 7.5);
    outColor = texture(tex, voxelCoord);
}