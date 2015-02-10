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

uniform vec3 Light2Pos;
uniform vec3 Light2Color;
uniform float Light2SpecularPower;

uniform vec3 Light1Pos;
uniform vec3 Light1Color;
uniform float Light1SpecularPower;


uniform vec3 Light3Direction;
uniform vec3 Light3Color;
uniform float Light3SpecularPower;

uniform vec3 CamPos;

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

vec3  CreatePointLight( vec3 Position, vec3 Normal, vec3 LightPos,vec3 LightColor, vec3 CamPos, vec3 diffuse, vec3 specular, float SpecularPower)
{
	vec3 l = normalize(LightPos - Position);
	float ndotl =  clamp(dot(normalize(Normal), l), 0.0, 1.0);

	vec3 PosCamVector = normalize(CamPos - Position);
	vec3 HalfVector = normalize(l + PosCamVector);
	float ndoth = clamp(dot(normalize(Normal), HalfVector), 0.0, 1.0);

	//vec3 color = mix(diffuse*ndotl, specular * pow(ndoth, SpecularPower), 0.7); ;
	
	vec3 color = diffuse *ndotl  +   specular * pow(ndoth, SpecularPower);
	color += LightColor;
	vec3 vecPosLight = (LightPos - Position);
	float attenuation = 1.0/sqrt(dot(vecPosLight,vecPosLight));
	return color*attenuation;
}
	
vec3  CreateDirectionalLight( vec3 Position, vec3 Normal, vec3 LightDirection,vec3 LightColor, vec3 CamPos, vec3 diffuse, vec3 specular, float SpecularPower)
{
	vec3 l = normalize(LightDirection);
	float ndotl =  clamp(dot(normalize(Normal), l), 0.0, 1.0);

	vec3 PosCamVector = normalize(CamPos - Position);
	vec3 HalfVector = normalize(l + PosCamVector);
	float ndoth = clamp(dot(normalize(Normal), HalfVector), 0.0, 1.0);

	//vec3 color = mix(diffuse*ndotl, specular * pow(ndoth, SpecularPower), 0.7); ;
	
	vec3 color = diffuse *ndotl  +   specular * pow(ndoth, SpecularPower);
	color += LightColor;
	/*vec3 vecPosLight = (LightPos - Position);
	float attenuation = 1.0/sqrt(dot(vecPosLight,vecPosLight));*/
	return color;
}
void main()
{
	
	////////////////////////////////////////////
	//TP3
	////////////////////////////////////////////
	//FragColor = vec4( In.Normal.x, In.Normal.y,In.Normal.z,1.0); // Normals
	//FragColor = vec4( texture(Diffuse, In.TexCoord).rgb, 1.0); // Diffuse
	//FragColor = vec4( texture(Specular, In.TexCoord).rgb, 1.0); // Specular
	//FragColor = vec4(SpecularPower/100.0, SpecularPower/100.0, SpecularPower/100.0 , 1.0); // SpecularPower
	//FragColor = vec4( 0.0, 0.0, gl_FragCoord.z , 1.0); // Profondeur
	Color = vec4( texture(Diffuse, In.TexCoord).rgb,texture(Specular, In.TexCoord).r ); // Color
	Normal = vec4( In.Normal,Light3SpecularPower); // Normals

	////////////////////////////////////////////
	//TP2
	////////////////////////////////////////////
/*	vec3 diffuse = texture(Diffuse, In.TexCoord).rgb;
	vec3 specular = texture(Specular, In.TexCoord).rgb;

	vec3 colorLight1 = CreatePointLight( In.Position, In.Normal, Light1Pos, Light1Color, CamPos, diffuse, specular, Light1SpecularPower);
	//vec3 colorLight2 = CreatePointLight( In.Position, In.Normal, Light2Pos, Light2Color, CamPos, diffuse, specular, Light2SpecularPower);
    FragColor = vec4(colorLight1 + colorLight2,1.0);
	/*vec3 colorLight3 = CreateDirectionalLight( In.Position, In.Normal, Light3Direction, Light3Color, CamPos, diffuse, specular, Light3SpecularPower);
		FragColor = vec4(colorLight3,1.0);
	float tmp = clamp(dot(normalize(In.OriginalNormal), vec3(1.0,0.0,0.0)), 0.0, 1.0);
		if(tmp == 1.0)
		{
			FragColor +=vec4(1.0,0.0,0.0, 1.0);
		}
//FragColor = vec4(specular,1.0);*/

/*
	////////////////////////////////////////////
	//TP1
	////////////////////////////////////////////
	//if(isCube)
	//{
		sin(200*In.TexCoord.x))),sin((20*In.TexCoord.y+cos(200*In.TexCoord.x))*tan(Time)),1.0);
		//FragColor = vec4( 0.5, sin(Time)*cos((20*In.TexCoord.y + 
	}
	else
	{
		FragColor = vec4( 0.0, In.TexCoord.x,In.TexCoord.y,1.0);
	}
	
*/
	/*
	////////////////////////////////////////////
	//TP0
	////////////////////////////////////////////
	//FragColor = vec4(In.Position.x, In.Position.y,In.Position.z,1.0);
	//FragColor = vec4( In.Normal.x, In.Normal.y,In.Normal.z,1.0);
	//FragColor = vec4( In.TexCoord.x,In.TexCoord.y,0.0, 1.0);
	
	*/	
	//FragColor = vec4( In.TexCoord.x,0.0,0.0, 1.0);
}
