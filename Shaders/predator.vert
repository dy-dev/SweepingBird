#version 420 core

#define POSITION	0
#define NORMAL		1
#define TEXCOORD	2
#define MATRIX_TRANSFORM 3

#define FRAG_COLOR	0

#define DIFFUSE_BINDING   0
#define SPECULAR_BINDING  1
#define AMBIANT_BINDING   2
#define OPACITY_BINDING   3
#define SHININESS_BINDING 4
#define PREDATORS_BINDING 5	

precision highp float;
precision highp int;

uniform mat4 VP;

uniform int InstanceNumber;
uniform vec3 BirdTranslation;

uniform float Time;
layout(binding=PREDATORS_BINDING) uniform samplerBuffer PredatorData;
layout(location = POSITION) in vec3 Position;
layout(location = NORMAL) in vec3 Normal;
layout(location = TEXCOORD) in vec2 TexCoord;
layout(location = MATRIX_TRANSFORM) in mat4 Transform;

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
	
	changePos *= 10;
	changePos += texelFetch(PredatorData, gl_InstanceID).rgb;
	
	gl_Position = VP * vec4(changePos, 1.0);
	
	Out.TexCoord = TexCoord;
	Out.Position = Position;
	Out.Normal = Normal;
}
