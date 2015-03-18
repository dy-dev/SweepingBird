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

uniform int ObjectId;

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
	vec3 changePos = Position;
	if(ObjectId == 0)
	{	
		float freq = MountainFrequence;
		if(freq == 0)
		{
			freq = 0.001;
		}
		
		float tempx = changePos.x/freq - Time;
		changePos.y = MaxMountainHeight*cos(tempx) + MaxMountainHeight*sin(changePos.z/freq);
		
		/*
		changePos.y = 	MaxMountainHeight*sin(tempx/freq) *sin(tempx/freq) + MaxMountainHeight*cos((changePos.z)/freq)*cos((changePos.z)/freq); 
		changePos.y += MaxMountainHeight*cos(5*changePos.z/freq)*cos(5*changePos.z/freq);
		changePos.y += MaxMountainHeight*sin(10*changePos.z/freq)*sin(10*changePos.z/freq)*sin(4*tempx/freq)*sin(4*tempx/freq);
		changePos.y += MaxMountainHeight*cos(15*changePos.z/freq)*cos(15*changePos.z/freq)*cos(8*tempx/freq)*cos(8*tempx/freq);
		changePos.y /= 2;*/	
	}
	else if(ObjectId == 1) // Bird
	{
		
	}
	else if(ObjectId == 2) // Bats
	{
		changePos.x += sin(gl_InstanceID) * gl_InstanceID;
		changePos.y += cos(gl_InstanceID) * gl_InstanceID;
		changePos.z += tan(gl_InstanceID) * gl_InstanceID;
		//changePos = vec3( cos(1.5f)* changePos.x -sin(1.5f)* changePos.z, changePos.y  , sin(1.5f)* changePos.x + cos(1.5f)* changePos.z);
	}
	
	
	gl_Position = MVP * vec4(changePos, 1.0);
	if(ObjectId == 3) //Skybox
	{
		vec4 WVP_Pos = MVP * vec4(changePos, 1.0);
		gl_Position = WVP_Pos.xyww;                   
	}
	Out.TexCoord = TexCoord;
	Out.Position = changePos;
	Out.Normal = Normal;
}