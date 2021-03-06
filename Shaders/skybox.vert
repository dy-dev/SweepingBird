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
uniform mat4 GroundTranslation;

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

	gl_Position = MVP * vec4(changePos, 1.0);
	vec4 tmp = GroundTranslation * vec4(changePos, 1.0);
	changePos = tmp.xyz;
	vec4 WVP_Pos = MVP * vec4(changePos, 1.0);
	gl_Position = WVP_Pos.xyww;                   
	Out.TexCoord = TexCoord;
	Out.Position = changePos;
	Out.Normal = Normal;
}
