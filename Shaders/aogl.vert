#version 410 core

#define POSITION	0
#define NORMAL		1
#define TEXCOORD	2
#define FRAG_COLOR	0

precision highp float;
precision highp int;

uniform mat4 MVP;
uniform mat4 MV;

uniform int InstanceNumber;
uniform vec3 Translation;
uniform vec3 BirdTranslation;

uniform float Time;

uniform bool isBird;
uniform bool isPredator;
uniform bool isGround;
uniform bool isSkybox;

uniform bool Rotate;
uniform float SizeFactor;
uniform float SpeedFactor;
uniform float RangeFactor;
uniform float RadiusSpacing;

uniform float MaxMountainHeight;
uniform float MountainFrequence;

layout(location = POSITION) in vec3 Position;
layout(location = NORMAL) in vec3 Normal;
layout(location = TEXCOORD) in vec2 TexCoord;


out gl_PerVertex
{
	vec4 gl_Position;
};

out block
{
	vec2 TexCoord;
	vec3 Position;
	vec3 OriginalNormal;
	vec3 Normal;
} Out;

void main()
{	
	vec3 changePos = Position;//*SizeFactor;
	changePos += Translation;
	if(isBird)
	{
		changePos = vec3(0.0);
	}
	else if(isPredator)
	{
		changePos.x += sin(gl_InstanceID) * gl_InstanceID;
		changePos.y += cos(gl_InstanceID) * gl_InstanceID;
		changePos.z += tan(gl_InstanceID) * gl_InstanceID;
	}
	else if(isSkybox)
	{
	}
	else if(isGround)
	{	
		float freq = MountainFrequence;
		if(freq == 0)
		{
			freq = 0.001;
		}
		changePos.x -= BirdTranslation.x;
		changePos.z -= BirdTranslation.z;
		
		changePos.y = MaxMountainHeight*cos(changePos.z);
	}
	gl_Position = MVP * vec4(changePos, 1.0);
	
	Out.TexCoord = TexCoord;
	Out.Position = changePos;
	Out.Normal = Normal;
}