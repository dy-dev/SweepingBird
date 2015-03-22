#version 420 core

#define POSITION	0
#define NORMAL		1
#define TEXCOORD	2
#define FRAG_COLOR	0

#define DIFFUSE_BINDING   0
#define SPECULAR_BINDING  1
#define AMBIANT_BINDING   2
#define OPACITY_BINDING   3
#define SHININESS_BINDING 4
#define PREDATORS_BINDING 5	

precision highp int;

layout(binding=DIFFUSE_BINDING) uniform sampler2D Diffuse;
layout(binding=SPECULAR_BINDING) uniform sampler2D Specular;


layout(location = FRAG_COLOR, index = 0) out vec4 FragColor;

uniform float Time;

in block
{
	vec2 TexCoord;
	vec3 Position;
	vec3 OriginalNormal;
	vec3 Normal;
} In;

void main()
{
	vec3 diffuse = texture(Diffuse, In.TexCoord).rgb;
	FragColor = vec4(diffuse,1.0);
}

