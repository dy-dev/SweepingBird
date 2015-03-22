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


precision highp float;
precision highp int;

uniform mat4 MVP;
uniform mat4 MV;

uniform int InstanceNumber;
uniform vec3 BirdTranslation;

uniform float Time;

layout(binding=PREDATORS_BINDING) uniform samplerBuffer PredatorData;

layout(location = POSITION) in vec3 Position;
layout(location = NORMAL) in vec3 Normal;
layout(location = TEXCOORD) in vec2 TexCoord;

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
	
	
		changePos *= 20;
		changePos += texelFetch(PredatorData, gl_InstanceID).rgb;
		/*
		changePos.x += sin(gl_InstanceID) * gl_InstanceID;
		changePos.y += cos(gl_InstanceID) * gl_InstanceID;
		changePos.z += tan(gl_InstanceID) * gl_InstanceID;
		*/
		//changePos = vec3( cos(1.5f)* changePos.x -sin(1.5f)* changePos.z, changePos.y  , sin(1.5f)* changePos.x + cos(1.5f)* changePos.z);
	
	gl_Position = MVP * vec4(changePos, 1.0);
	
	Out.TexCoord = TexCoord;
	Out.Position = Position;
	Out.Normal = Normal;
}
