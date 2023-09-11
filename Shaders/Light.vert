#version 330 core
#extension GL_ARB_shading_language_include : require

layout (location = 0) in vec3 aPos;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
	gl_Position = Projection * View * Model * vec4(aPos, 1.0);
}