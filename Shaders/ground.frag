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
layout(binding=AMBIANT_BINDING) uniform sampler2D Ambiant;
layout(binding=OPACITY_BINDING) uniform sampler2D Opacity;
layout(binding=SHININESS_BINDING) uniform sampler2D Shininess;

layout(location = FRAG_COLOR, index = 0) out vec4 FragColor;

// Write in GL_COLOR_ATTACHMENT0
layout(location = 0 ) out vec4 Color;
// Write in GL_COLOR_ATTACHMENT1
layout(location = 1) out vec4 Normal;

uniform float Time;
uniform float MaxMountainHeight;

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
	vec3 color = diffuse;
	vec3 specular = texture(Specular, In.TexCoord).rgb;
	vec3 ambiant = texture(Ambiant, In.TexCoord).rgb;
	vec3 opacity = texture(Opacity, In.TexCoord).rgb;
	vec3 shininess = texture(Shininess, In.TexCoord).rgb;
	
	if ( In.Position.y < MaxMountainHeight/2)
	{
		color = mix(diffuse, specular, 2*In.Position.y/(MaxMountainHeight)); 
	}
	else if (In.Position.y > MaxMountainHeight/2 && In.Position.y < MaxMountainHeight)
	{
		color = mix(specular, ambiant, (2*In.Position.y - MaxMountainHeight)/MaxMountainHeight); 
	}
	else if (In.Position.y > MaxMountainHeight && In.Position.y < 3*MaxMountainHeight/2)
	{
		color = mix(ambiant, opacity, 2*(In.Position.y - MaxMountainHeight)/MaxMountainHeight); 
	}
	else if (In.Position.y > 3*MaxMountainHeight/2 && In.Position.y < 2*MaxMountainHeight)
	{
		color = mix(opacity, shininess, 2*(In.Position.y - 3*MaxMountainHeight/2)/MaxMountainHeight);
	}
	else if (In.Position.y > 2*MaxMountainHeight)
	{
		color = shininess; 
	}
		
	
	FragColor = vec4(color,1.0);
}

