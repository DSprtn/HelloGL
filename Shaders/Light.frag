#version 330 core

#include "LightData.glsl"

out vec4 FragColor;

uniform vec3 LightCol;

void main()
{
	FragColor = vec4(LightCol,1.0);
}