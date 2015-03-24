#version 430 core

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
#define HEIGHTMAP_BINDING 6
#define NORMALMAP_BINDING 7

layout(binding=PREDATORS_BINDING) uniform samplerBuffer PredatorData;
layout(binding=HEIGHTMAP_BINDING) uniform samplerBuffer HeightMapData;
layout(binding=NORMALMAP_BINDING) uniform samplerBuffer NormalMapData;

layout(location = POSITION) in vec3 Position;
layout(location = NORMAL) in vec3 Normal;
layout(location = TEXCOORD) in vec2 TexCoord;

precision highp float;
precision highp int;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 GroundTranslation;

uniform int InstanceNumber;
uniform int SquareSideLength;
uniform vec3 Translation;

uniform float Time;


uniform float SizeFactor;
uniform float RangeFactor;
uniform float RadiusSpacing;
uniform float MaxMountainHeight;
uniform float MountainFrequence;
uniform float PatchControl;


uniform float m_ftest1;
uniform float m_ftest2;
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

 float Noise(int x,int y)
{
	int n = x + y * 57;
	n = (n << 13) ^ n;
	int t = (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff;
	return 1.0 - float(t) * 0.931322574615478515625e-9;/// 1073741824.0);
}

float Interpolate(float x,float y,float a)
{
	float negA = 1.0 - a;
	float negASqr = negA * negA;
	float fac1 = 3.0 * (negASqr) - 2.0 * (negASqr * negA);
	float aSqr = a * a;
	float fac2 = 3.0 * aSqr - 2.0 * (aSqr * a);

	return x * fac1 + y * fac2; //add the weighted factors
}

float GetValue(float x,float y)
{
	int Xint = int(x);
	int Yint = int(y);
	float Xfrac = x - Xint;
	float Yfrac = y - Yint;

	//noise values
	float n01 = Noise(Xint-1, Yint-1);
	float n02 = Noise(Xint+1, Yint-1);
	float n03 = Noise(Xint-1, Yint+1);
	float n04 = Noise(Xint+1, Yint+1);
	float n05 = Noise(Xint-1, Yint);
	float n06 = Noise(Xint+1, Yint);
	float n07 = Noise(Xint, Yint-1);
	float n08 = Noise(Xint, Yint+1);
	float n09 = Noise(Xint, Yint);

	float n12 = Noise(Xint+2, Yint-1);
	float n14 = Noise(Xint+2, Yint+1);
	float n16 = Noise(Xint+2, Yint);

	float n23 = Noise(Xint-1, Yint+2);
	float n24 = Noise(Xint+1, Yint+2);
	float n28 = Noise(Xint, Yint+2);

	float n34 = Noise(Xint+2, Yint+2);

	//find the noise values of the four corners
	float x0y0 = 0.0625*(n01+n02+n03+n04) + 0.125*(n05+n06+n07+n08) + 0.25*(n09);  
	float x1y0 = 0.0625*(n07+n12+n08+n14) + 0.125*(n09+n16+n02+n04) + 0.25*(n06);  
	float x0y1 = 0.0625*(n05+n06+n23+n24) + 0.125*(n03+n04+n09+n28) + 0.25*(n08);  
	float x1y1 = 0.0625*(n09+n16+n28+n34) + 0.125*(n08+n14+n06+n24) + 0.25*(n04);  

//interpolate between those values according to the x and y fractions
	float v1 = Interpolate(x0y0, x1y0, Xfrac); //interpolate in x direction (y)
	float v2 = Interpolate(x0y1, x1y1, Xfrac); //interpolate in x direction (y+1)
	float fin = Interpolate(v1, v2, Yfrac);  //interpolate in y direction

	return fin;
	}

float Total(float i, float j, float _amplitude, float frequency, float persistence, float freqdiv ) 
{
    //properties of one octave (changing each loop)
    float t = 0.0f;
	
    float amplitude = _amplitude;
    float freq = frequency;
   
   
	t += GetValue(j * freq , i * freq ) * amplitude;
	amplitude *= persistence;
	freq *= freqdiv;

	t += GetValue(j * freq , i * freq ) * amplitude;
	amplitude *= persistence;
	freq *= freqdiv;

	t += GetValue(j * freq , i * freq ) * amplitude;

    return t;
}

void main()
{	
	vec3 changePos = Position;
	
	int divider = SquareSideLength;
	if(SquareSideLength == 0)
	{
		divider = 1;
	}
	/*float xGridCood = PatchControl * (gl_InstanceID%divider) - PatchControl * divider/2 ;
	float zGridCood = PatchControl * (gl_InstanceID/divider) - PatchControl* divider/2;*/
	float xGridCood = 1800*(gl_InstanceID%divider - divider/2.0 );
	float zGridCood = 1800*(gl_InstanceID/divider - divider/2.0);
	float height =0;
	if(gl_InstanceID == 4)
	{
		height = texelFetch(HeightMapData,gl_VertexID).x;
		Out.Normal = texelFetch(NormalMapData,gl_VertexID).xyz;
	}
	else if(gl_InstanceID == 5)
	{
		height = texelFetch(HeightMapData,gl_VertexID + (gl_InstanceID-4)*961).x;	
		Out.Normal = texelFetch(NormalMapData,gl_VertexID + (gl_InstanceID-4)*961).xyz;
	}
	else if(gl_InstanceID == 6)
	{
		height = texelFetch(HeightMapData,gl_VertexID + (gl_InstanceID-4)*961).x;
		Out.Normal = texelFetch(NormalMapData,gl_VertexID+ (gl_InstanceID-4)*961).xyz;
	}
	else if(gl_InstanceID == 7)
	{
		height = texelFetch(HeightMapData,gl_VertexID + (gl_InstanceID-4)*961).x;
		Out.Normal = texelFetch(NormalMapData,gl_VertexID+ (gl_InstanceID-4)*961).xyz;
	}
	
	else
	{
		//float xVertexCoord = (gl_VertexID%30);
		//float zVertexCoord = (gl_VertexID/30);

		//height = Total(xVertexCoord, zVertexCoord,m_ftest1,m_ftest2, MountainFrequence,PatchControl);
		//height = Total(changePos.x+xGridCood, changePos.z+zGridCood,m_ftest1,m_ftest2, MountainFrequence,PatchControl);
		Out.Normal = Normal;
	}
	
	changePos.x += xGridCood;
	changePos.z += zGridCood;
	/*vec4 tmp = GroundTranslation * vec4(changePos, 1.0);
	changePos = tmp.xyz;
	/*float tmpNoise = Total(abs(changePos.x ), abs(changePos.z), 2,0.0005);
	
	
	int freq = int(MountainFrequence);
	if(freq == 0)
	{
		freq = 1;
	}
	
	float tempx = changePos.x/(10*freq);// - Time;
	float tempz = changePos.z/(10*freq);
	
	changePos.y = MaxMountainHeight*(cos(tempx)*cos(2.0*tempx)*sin(4.0*tempz) + sin(tempz + 1.5)*sin(2.0*tempz)*cos(tempx*8.0));
	changePos.y += MaxMountainHeight*(sin(tempz/5.0)*cos(3.0*tempx) + sin(tempx)*sin(5*tempz));
	changePos.y += MaxMountainHeight*(cos(tempz+1.5)*sin(tempz)*cos(9.0*tempx) + cos(tempx+1.5)*cos(tempz/5.0)*sin(tempx*5.0));
	
	changePos.y = MaxMountainHeight*tmpNoise;
	*/
	changePos.y = MaxMountainHeight*height;
	if(gl_InstanceID == 0)
	{
		changePos.y += 100; 
	}
	gl_Position = MVP * vec4(changePos, 1.0);
	Out.TexCoord = TexCoord;
	Out.Position = changePos;
	
}
