#version 330 core

#include "MaterialCommon.glsl"

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 aTangent;

out VS_OUT {
	vec2 textureCoord;
	vec3 fragViewPos;
	mat3 TBN;
} vs_out;  

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Normal;

void main()
{
	gl_Position = Projection * View * Model * vec4(aPos, 1.0);
	vec4 pos = Model * vec4(aPos, 1.0);
	vs_out.textureCoord = texCoord;

	vec3 T = normalize(vec3(View * Model * vec4(aTangent,0.0)));
	vec3 N = normalize(vec3(View * Model * vec4(aNormal,0.0)));
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);
	mat3 TBN = mat3(T, B, N);

	vs_out.TBN = TBN;
	vs_out.fragViewPos = vec3(View * Model * vec4(aPos,1.0));
}
