#version 410 core
in blockBlit
{
    vec2 Texcoord;
} InBlit; 

uniform sampler2D Texture;
layout(location = 0, index = 0) out vec4  Color;

void main(void)
{
    vec3 color = texture(Texture, InBlit.Texcoord).rgb;
    Color = vec4(color, 1.0);
}