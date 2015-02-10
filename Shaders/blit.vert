#version 410 core
#define POSITION 0
layout(location = POSITION) in vec2 BlitPosition;

out blockBlit
{
    vec2 Texcoord;
} OutBlit;

void main()
{   
    OutBlit.Texcoord = BlitPosition * 0.5 + 0.5;
    gl_Position = vec4(BlitPosition.xy, 0.0, 1.0);
}