#version 410 core

#define POSITION	0
#define NORMAL		1
#define TEXCOORD	2
#define FRAG_COLOR	0

precision highp float;
precision highp int;

uniform mat4 MVP;

uniform mat4 ObjectToLightScreen;
uniform mat4 ObjectToLight;

uniform int InstanceNumber;

uniform float Time;

uniform bool isCube;
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
	//vec3 transPosition = vec3(Position.x + gl_InstanceID/5.0 ,Position.y + 5*sin(gl_InstanceID/5.0), Position.z );
	vec3 transPosition = vec3(Position.x ,Position.y , Position.z );
	vec3 changedPos;
	
	vec3 transNormal = Normal;
	if(isCube)
	{
		transPosition *= SizeFactor;
		//transPosition = vec3( cos(Time)* transPosition.x -sin(Time)* transPosition.z, transPosition.y + gl_InstanceID*sin(Time/2)/100.0  , sin(Time)* transPosition.x + cos(Time)* transPosition.z);
		float radius = 1.0;
		float angle = 2.0*3.14/ 8.0;
		if(gl_InstanceID == 0)
		{
			transPosition = vec3(Position.x ,Position.y , Position.z );
			transPosition *= SizeFactor;
			radius = 0.0;
		}
		else
		{
			int prev = 0;
			int i = 0;
			bool found = false;
			while(found == false)
			{
				if(gl_InstanceID > prev && gl_InstanceID <= prev + 8*i)
				{
					radius =i;
					angle *= (gl_InstanceID-prev)/radius;
					found = true;
					break;
				}
				prev += 8*i;
				i++;
			}
		}
		
		transPosition.x += radius*SizeFactor*cos(angle)*RadiusSpacing;
		transPosition.z += radius*SizeFactor*sin(angle)*RadiusSpacing;
		
		//changedPos = transPosition
		// changedPos = vec3( cos(Time)* transPosition.x -sin(Time)* transPosition.z, transPosition.y + gl_InstanceID*sin(Time/2)/100.0  , sin(Time)* transPosition.x + cos(Time)* transPosition.z);
		float changedTime=Time*SpeedFactor;
		changedPos = vec3( transPosition.x, transPosition.y - RangeFactor*cos(radius*SizeFactor*RadiusSpacing*2 - changedTime)  , transPosition.z);
		transNormal = vec3( transNormal.x, transNormal.y - RangeFactor*cos(radius*SizeFactor*RadiusSpacing*2 - changedTime), transNormal.z);
		if(Rotate)
		{
			if(mod(radius,4) == 0)
			{
				changedPos = vec3( cos(changedTime)* changedPos.x -sin(changedTime)* changedPos.y, sin(changedTime)* changedPos.x + cos(changedTime)* changedPos.y , changedPos.z );
				transNormal = vec3( cos(changedTime)* transNormal.x -sin(changedTime)* transNormal.y, sin(changedTime)* transNormal.x + cos(changedTime)* transNormal.y , transNormal.z );
		
			}
			else if(mod(radius,3) == 0)
			{
				changedPos = vec3( -cos(changedTime)* changedPos.x + sin(changedTime)* changedPos.y, -sin(changedTime)* changedPos.x - cos(changedTime)* changedPos.y , changedPos.z );
				transNormal = vec3(-cos(changedTime)* transNormal.x +sin(changedTime)* transNormal.y, -sin(changedTime)* transNormal.x - cos(changedTime)* transNormal.y , transNormal.z );
			}
		}
		// transNormal = vec3( cos(Time)* transNormal.x -sin(Time)* transNormal.z, transNormal.y  , sin(Time)* transNormal.x + cos(Time)* transNormal.z);
	/*	 changedPos = vec3( transPosition.x + Time,  transPosition.y +sin(Time),  transPosition.z);
		 if(changedPos.x > 25 )
		 {
		 	changedPos.x = Position.x;
			changedPos.y = Position.y;
			changedPos.z = Position.z;
		 }*/
	}
	else
	{
	   changedPos = transPosition;
	}
	
	gl_Position = MVP * vec4(changedPos, 1.0);
	Out.TexCoord = TexCoord;
	Out.Position = changedPos;
	Out.OriginalNormal=Normal;
	Out.Normal = transNormal;
}