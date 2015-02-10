#version 410 core

precision highp float;
precision highp int;
layout(std140, column_major) uniform;
layout(triangles) in;
layout(triangle_strip, max_vertices = 4) out;

in block
{
	vec2 TexCoord;
	vec3 Position;
	vec3 Normal;
} In[]; 

out block
{
    vec2 TexCoord;
	vec3 Position;
	vec3 Normal;
}Out;

uniform mat4 MVP;
uniform mat4 MV;
uniform float Time;

void main()
{   
    for(int i = 0; i < gl_in.length(); ++i)
    {
        gl_Position = gl_in[i].gl_Position;;
        Out.TexCoord = In[i].TexCoord;
        EmitVertex();
    }
    EndPrimitive();
}