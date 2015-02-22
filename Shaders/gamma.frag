#version 410 core

in blockBlit
{
	vec2 Texcoord;
} In;

uniform sampler2D Texture;
uniform float Gamma = 1.0;

layout(location = 0, index = 0) out vec4 Color;

void main(void)
{
	vec3 color = texture(Texture, In.Texcoord).rgb;
	Color = vec4(pow(color.r,1.0/Gamma),pow(color.g,1.0/Gamma),pow(color.b,1.0/Gamma), 1.0);
}