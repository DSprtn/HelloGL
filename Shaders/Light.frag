#version 330 core

out vec4 FragColor;

uniform vec3 LightCol;

void main()
{
	FragColor = vec4(LightCol,1.0);
}