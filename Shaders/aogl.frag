#version 410 core

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
layout(binding=PREDATORS_BINDING) uniform sampler2D Shininess;

uniform bool isBird;
uniform bool isPredator;
uniform bool isGround;
uniform bool isSkybox;
uniform float Time;
uniform int ObjectId;
uniform float MaxMountainHeight;


uniform vec3 Light1Pos;
uniform vec3 Light1Color;
uniform float Light1SpecularPower;
uniform bool Light1Use;

uniform vec3 Light2Pos;
uniform vec3 Light2Color;
uniform float Light2SpecularPower;
uniform bool Light2Use;

uniform vec3 Light3Direction;
uniform vec3 Light3Color;
uniform float Light3SpecularPower;
uniform bool Light3Use;

uniform vec3 SpotLightPosition;
uniform vec3 SpotLightDirection;
uniform vec3 SpotLightColor;
uniform float SpotLightAngle;
uniform float SpotLightFallOffAngle;
uniform float SpotLightIntensity;
uniform float SpotLightSpecularPower;
uniform bool SpotLightUse;

uniform vec3 CamPos;

uniform float ColorControl;

layout(location = FRAG_COLOR, index = 0) out vec4 FragColor;

// Write in GL_COLOR_ATTACHMENT0
layout(location = 0 ) out vec4 Color;
// Write in GL_COLOR_ATTACHMENT1
layout(location = 1) out vec4 Normal;


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
	if(ObjectId == 0)
	{
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
		
	}
	/*
		diffuse =  vec3(0.1f,0.1f,0.7f);
		if(In.Position.y < 20)
		{
			diffuse =  vec3(0.1f,0.1f,0.7f);
		}
		if(In.Position.y >20 && In.Position.y < 200)
		{
			diffuse =  vec3(0.8f,0.8f,0.3f);
		}
		else if(In.Position.y > 200 && In.Position.y < 500)
		{
			diffuse =  vec3(0.1f,0.6f,0.0f);
		}
		else if(In.Position.y > 500 && In.Position.y < 1000)
		{
			diffuse =  vec3(0.9f,0.9f,0.0f);
		}
		
		else if(In.Position.y > 1000 && In.Position.y < 2000)
		{
			diffuse =  vec3(0.5f,0.5f,0.5f);
		}
		else if(In.Position.y > 2000)
		{
			diffuse =  vec3(0.9f,0.9f,0.9f);
		}
		float red = 0.0;
		float green = 0.0;
		float blue = 1.0;
		
		if(In.Position.y < 1000)
		{
			blue -= In.Position.y/ColorControl;
		}
		else
		{
			blue = (In.Position.y-1000)/ColorControl - 1.0;
		}
		
		if(In.Position.y < 200)
		{
			green = 3*In.Position.y/ColorControl;
			red =  In.Position.y/ColorControl;
		}
		else //if(In.Position.y > 200 && In.Position.y < 500)
		{
			green = (3*In.Position.y-200)/ColorControl - 1.0;
			 red = 2*In.Position.y/ColorControl;
		}
		
		diffuse =  vec3(red,green,blue);
	}*/
	FragColor = vec4(color,1.0);
}

