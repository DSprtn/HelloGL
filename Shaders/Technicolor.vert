#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 aTangent;

out VS_OUT {
	vec3 vertexColor;
	vec3 normal;
	vec2 textureCoord;
	vec3 worldPos;
	vec4 viewSpacePos;
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
	vs_out.worldPos = vec3(pos.x,pos.y,pos.z);
	vs_out.viewSpacePos = View * Model * vec4(aPos,1.0);
	vs_out.normal = mat3(View) * mat3(Normal) * aNormal;
	vs_out.textureCoord = texCoord;

	vec3 bitangent = cross(aNormal, aTangent);

	vec3 T = normalize(vec3(Model * vec4(aTangent,   0.0)));
	vec3 B = normalize(vec3(Model * vec4(bitangent, 0.0)));
	vec3 N = normalize(vec3(Model * vec4(aNormal,    0.0)));
	vs_out.TBN = transpose(mat3(T, B, N));
}