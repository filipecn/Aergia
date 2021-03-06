#version 150

out vec3 texCoord;
flat out int instanceID;

void main()
{
    float x = -1.0 + float((gl_VertexID & 1) << 2);
    float y = -1.0 + float((gl_VertexID & 2) << 1);
    instanceID  = gl_InstanceID;
    texCoord.x = (x+1.0)*0.5;
    texCoord.y = (y+1.0)*0.5;
    texCoord.z = 0.0;

    gl_Position = vec4(x, y, 0, 1);
}