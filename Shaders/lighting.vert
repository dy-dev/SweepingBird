#version 410 core
#define POSITION 0
layout(location = POSITION) in vec2 LightingPosition;

out blockLighting
{
    vec2 Texcoord;
} OutLighting;

void main()
{   
    OutLighting.Texcoord = LightingPosition * 0.5 + 0.5;
    gl_Position = vec4(LightingPosition.xy, 0.0, 1.0);
}