#version 330 core

out vec4 FragColor;
in vec3 vertexColor;
in vec2 textureCoord;

uniform vec4 globalCol;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;

void main()
{
	vec4 texColor = texture(ourTexture, textureCoord);
	vec4 tex2Color = texture(ourTexture2, textureCoord);
	FragColor = mix(texColor, tex2Color, globalCol.a);
}