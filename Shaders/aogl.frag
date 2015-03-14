#version 410 core

#define POSITION	0
#define NORMAL		1
#define TEXCOORD	2
#define FRAG_COLOR	0

precision highp int;

uniform sampler2D Diffuse;
uniform sampler2D Specular;

uniform bool isCube;
uniform float Time;

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
	if(isCube)
	{
		float colorChange = ColorControl;
		if(colorChange ==0.0)
		{
			colorChange = 0.001;
		}
		float GroundColor = 1.0-In.Position.y/colorChange;	
		diffuse =  vec3(GroundColor,GroundColor,GroundColor);
	}
	FragColor = vec4(diffuse,1.0);
}
