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

uniform float Time;

uniform bool isCube;
uniform bool isLight1Marker;
uniform bool isLight2Marker;
uniform bool isSpotLightMarker;
uniform vec3 Light1Pos;
uniform vec3 Light2Pos;
uniform vec3 SpotLightPosition;

uniform bool Rotate;
uniform float SizeFactor;
uniform float SpeedFactor;
uniform float RangeFactor;
uniform float RadiusSpacing;

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
	
	gl_Position = MVP * vec4(Position, 1.0);
	Out.TexCoord = TexCoord;
	Out.Position = Position;
	Out.Normal = Normal;
}