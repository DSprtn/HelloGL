#version 330 core

#include "common.glsl"

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 aTangent;

out VS_OUT {
	vec3 vertexColor;
	vec2 textureCoord;
	vec3 worldPos;
	vec3 fragTangentPos;
	vec3 fragViewPos;
	mat3 TBN;
	vec3 pointLightPositions[POINT_LIGHT_COUNT];
	vec3 directionalLightDir;
	vec3 spotlightPos;
	vec3 spotLightDir;
} vs_out;  

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Normal;

void main()
{
	gl_Position = Projection * View * Model * vec4(aPos, 1.0);
	vec4 pos = Model * vec4(aPos, 1.0);
	vs_out.worldPos = vec3(Model * vec4(pos.x,pos.y,pos.z,1));
	vs_out.textureCoord = texCoord;

	
	vec3 T = normalize(aTangent);
	vec3 N = normalize(aNormal);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);
	mat3 TBN = transpose(mat3(Model) * mat3(T, B, N));

	vs_out.TBN = TBN;

	for(int i = 0; i < POINT_LIGHT_COUNT; i++)
	{
		vs_out.pointLightPositions[i] = TBN * pointLights[i].position;
	}

	vs_out.directionalLightDir = TBN * directionalLight.direction;
	vs_out.spotLightDir = TBN * spotlight.direction;
	vs_out.spotlightPos = TBN * spotlight.position;

	vs_out.fragTangentPos = TBN * vec3(View * Model * vec4(aPos,1.0));
	vs_out.fragViewPos = vec3(View * Model * vec4(aPos, 1.0));
}
