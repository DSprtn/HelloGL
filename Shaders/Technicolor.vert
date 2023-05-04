#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoord;

out vec3 vertexColor;
out vec3 normal;
out vec2 textureCoord;
out vec3 worldPos;
out vec4 viewSpacePos;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Normal;

void main()
{
	gl_Position = Projection * View * Model * vec4(aPos, 1.0);
	vec4 pos = Model * vec4(aPos, 1.0);
	worldPos = vec3(pos.x,pos.y,pos.z);
	viewSpacePos = View * Model * vec4(aPos,1.0);
	normal = mat3(View) * mat3(Normal) * aNormal;  
	textureCoord = texCoord;
}