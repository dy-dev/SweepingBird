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

uniform vec3 LightDirection;
uniform vec3 LightColor;
uniform float LightSpecularPower;
uniform bool LightUse;

uniform vec3 CamPos;

in block
{
	vec2 TexCoord;
	vec3 Position;
	vec3 OriginalNormal;
	vec3 Normal;
} In;
	
vec3  CreateDirectionalLight( vec3 pos, vec3 norm, vec3 lightDirection ,vec3 lightColor, float specularPower, vec3 camPos, vec3 diffuse, vec3 specular)
{
	vec3 l = normalize(lightDirection);
	float ndotl =  clamp(dot(normalize(norm), l), 0.0, 1.0);

	vec3 PosCamVector = normalize(camPos - pos);
	vec3 HalfVector = normalize(l + PosCamVector);
	float ndoth = clamp(dot(normalize(norm), HalfVector), 0.0, 1.0);

	vec3 color = mix(diffuse*ndotl, specular * pow(ndoth, specularPower), 0.7); ;
	
	//vec3 color = diffuse *ndotl  +   specular * pow(ndoth, specularPower);
	color += lightColor;
//	vec3 vecPosLight = (LightPos - Position);
//float attenuation = 1.0/dot(vecPosLight,vecPosLight);
	return color;
}

void main()
{
	vec3 diffuse = texture(Diffuse, In.TexCoord).rgb;
	vec3 specular = texture(Specular, In.TexCoord).rgb;
	vec3 ambiant = texture(Ambiant, In.TexCoord).rgb;
	vec3 opacity = texture(Opacity, In.TexCoord).rgb;
	vec3 shininess = texture(Shininess, In.TexCoord).rgb;
	vec3 color = vec3(0);
	if ( In.Position.y < MaxMountainHeight/4)
	{
		color = mix(diffuse, specular, 4*In.Position.y/(MaxMountainHeight)); 
	}
	else if (In.Position.y > MaxMountainHeight/4 && In.Position.y < MaxMountainHeight/2)
	{
		color = mix(specular, ambiant, 4*(In.Position.y - MaxMountainHeight/4)/MaxMountainHeight); 
	}
	else if (In.Position.y > MaxMountainHeight/2 && In.Position.y < 3*MaxMountainHeight/4)
	{
		color = mix(ambiant, opacity, 4*(In.Position.y - MaxMountainHeight/2)/MaxMountainHeight); 
	}
	else if (In.Position.y > 3*MaxMountainHeight/4 && In.Position.y < MaxMountainHeight)
	{
		color = mix(opacity, shininess, 4*(In.Position.y - 3*MaxMountainHeight/4)/MaxMountainHeight);
	}
	else if (In.Position.y > MaxMountainHeight)
	{
		color = shininess; 
	}
		
	FragColor = vec4(color,1.0);
	if(LightUse)
	{
		vec3 dirLight = CreateDirectionalLight( In.Position, In.Normal, LightDirection, LightColor, LightSpecularPower, CamPos, color, specular);
		FragColor += vec4(dirLight, 1.0);
	}
	else{
		//FragColor = vec4(In.Normal,1.0);
	}
}

