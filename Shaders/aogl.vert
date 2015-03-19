#version 410 core

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
uniform int SquareSideLength;
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
uniform float ColorControl;
uniform float MaxMountainHeight;
uniform float MountainFrequence;

layout(binding=PREDATORS_BINDING) uniform samplerBuffer PredatorData;

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

 float findnoise2(int x,int y)
{
 int n= x + y*57;
 n=(n<<13)^n;
 int nn=(n * (n * n * 60493+19990303)+1376312589) & 0x7fffffff;
 
 return 1.0-(float(nn)/1073741824.0);
}

float interpolate(float a,float b,float x)
{
  float     pi_mod;
  float     f_unk;

  pi_mod = x * 3.1415927;
  f_unk = (1 - cos(pi_mod)) * 0.5;
  return (a * (1 - f_unk) + b * x);
}

float noise(float x,float y)
{
 int floorx = int(floor(x));
 int floory = int(floor(y));
 float s=findnoise2(floorx,floory); 
 float t=findnoise2(floorx+1,floory);
 float u=findnoise2(floorx,floory+1);//Get the surrounding pixels to calculate the transition.
 float v=findnoise2(floorx+1,floory+1);
 float int1=interpolate(s,t,0.5);//Interpolate between the values.
 float int2=interpolate(u,v,0.5);//Here we use x-floorx, to get 1st dimension. Don't mind the x-floorx thingie, it's part of the cosine formula.
 return interpolate(int1,int2,0.4);//Here we use y-floory, to get the 2nd dimension.
}

void main()
{	
	vec3 changePos = Position;
	if(ObjectId == 0)
	{	
		float xGridCood = ColorControl * (gl_InstanceID%SquareSideLength) - ColorControl * SquareSideLength/2;
		float zGridCood = ColorControl * (gl_InstanceID/SquareSideLength) - ColorControl * SquareSideLength/2;
		float tmpNoise = 0.0;
		
		changePos.x += xGridCood;
		changePos.z += zGridCood;
		
		int freq = int(MountainFrequence);
		if(freq == 0)
		{
			freq = 1;
		}
		
		float tempx = changePos.x/(5*freq) - Time;
		float tempz = changePos.z/(5*freq);
		
		changePos.y = MaxMountainHeight*(cos(tempx)*cos(2.0*tempx)*sin(4.0*tempz) + sin(tempz + 1.5)*sin(2.0*tempz)*cos(tempx*8.0));
		changePos.y += MaxMountainHeight*(sin(tempz/5.0)*cos(3.0*tempx) + sin(tempx)*sin(5*tempz));
		changePos.y += MaxMountainHeight*(cos(tempz+1.5)*sin(tempz)*cos(9.0*tempx) + cos(tempx+1.5)*cos(tempz/5.0)*sin(tempx*5.0));

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
		changePos = texelFetch(PredatorData, gl_InstanceID).rgb;
		/*
		changePos.x += sin(gl_InstanceID) * gl_InstanceID;
		changePos.y += cos(gl_InstanceID) * gl_InstanceID;
		changePos.z += tan(gl_InstanceID) * gl_InstanceID;
		*/
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
