#version 410 core
in blockLighting
{
    vec2 Texcoord;
} In; 

uniform sampler2D ColorBuffer;
uniform sampler2D NormalBuffer;
uniform sampler2D DepthBuffer;

uniform sampler2D ShadowBuffer;

uniform vec3 Light3Direction;
uniform vec3 Light3Color;
uniform float Light3SpecularPower;


// Write in GL_COLOR_ATTACHMENT0
layout(location = 0 ) out vec4 Color;
// Write in GL_COLOR_ATTACHMENT1
layout(location = 1) out vec4 Normal;	

uniform mat4 ScreenToWorld;
uniform vec3 Light1Pos;
uniform vec3 Light1Color;
uniform vec3 CamPos;


vec3  CreateLight( vec3 Position, vec3 Normal, vec3 LightPos,vec3 LightColor, vec3 CamPos, vec3 diffuse, vec3 specular, float SpecularPower)
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
	
	vec3 color = diffuse*ndotl  +   specular * pow(ndoth, SpecularPower);
	//color += LightColor;
	/*vec3 vecPosLight = (LightPos - Position);
	float attenuation = 1.0/sqrt(dot(vecPosLight,vecPosLight));*/
	
	return color;
}

void main(void)
{
	// Read gbuffer values
	vec4 colorBuffer = texture(ColorBuffer, In.Texcoord).rgba;
	vec4 normalBuffer = texture(NormalBuffer, In.Texcoord).rgba;
	float depth = texture(DepthBuffer, In.Texcoord).r;
	
	// Unpack values stored in the gbuffer
	vec3 diffuseColor = colorBuffer.rgb;
	vec3 specularColor = colorBuffer.aaa;
	float specularPower = normalBuffer.a;
	vec3 n = normalBuffer.rgb;

	// Convert texture coordinates into screen space coordinates
	vec2 xy = In.Texcoord * 2.0 -1.0;
	// Convert depth to -1,1 range and multiply the point by ScreenToWorld matrix
	vec4 wP = vec4(xy, depth * 2.0 -1.0, 1.0) * ScreenToWorld;
	// Divide by w
	vec3 p = vec3(wP.xyz / wP.w);


	
	/*vec3 l = normalize(Light1Pos - p);
	float ndotl =  clamp(dot(normalize(n), l), 0.0, 1.0);

	vec3 PosCamVector = normalize(CamPos - p);
	vec3 HalfVector = normalize(l + PosCamVector);
	float ndoth = clamp(dot(normalize(n), HalfVector), 0.0, 1.0);

	
	vec3 color = diffuseColor *ndotl  +   specularColor * pow(ndoth, specularPower);
	//color += Light1Color;
	float vecPosLight = distance(Light1Pos, p);*/
	float attenuation = 1.0;///(vecPosLight*vecPosLight);
	
	
	//vec3 color = CreateLight(p, n, Light1Pos,Light1Color, CamPos, diffuseColor, specularColor,  specularPower);
	vec3 color = CreateDirectionalLight( p, n, Light3Direction, Light3Color,  CamPos, diffuseColor, specularColor,  specularPower);
	float shadowDepth = texture(ShadowBuffer, gl_FragCoord.xy).r;
	Color = vec4( 0.0, 0.0, depth	 , 1.0); // Profondeur
	//Color = vec4(specularPower/255.0,specularPower/255.0,specularPower/255.0,1.0);
	
	//Color = vec4(vecPosLight,vecPosLight,vecPosLight,1.0);
	//Color = vec4(attenuation,attenuation,attenuation,1.0);
	//Color = vec4(color*attenuation,1.0);
}