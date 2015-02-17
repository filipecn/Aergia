#version 150 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in int instanceID[3];
out float gLayer;

void main()
{
    gl_Layer = instanceID[0];
    gLayer = float(gl_Layer) + 0.5;
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();
    EndPrimitive();
}