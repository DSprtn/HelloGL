#version 330 core
#extension GL_ARB_shading_language_include : require

out vec4 FragColor;

uniform vec3 LightCol;

void main()
{
	FragColor = vec4(LightCol,1.0);
}