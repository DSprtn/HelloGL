#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 vecColor;
layout (location = 2) in vec2 texCoord;

out vec3 vertexColor;
out vec2 textureCoord;

uniform mat4 transform;
uniform mat4 MVP;

void main()
{
	gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	vertexColor = vecColor;
	textureCoord = texCoord;
}