#version 330 core

out vec4 FragColor;
in vec3 vertexColor;
in vec2 textureCoord;
in vec3 normal;
in vec3 worldPos;
in vec4 viewSpacePos;

uniform vec4 globalCol;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;

uniform vec3 LightCol;
uniform vec3 LightPos;

void main()
{
	vec4 texColor = texture(ourTexture, textureCoord);
	vec4 tex2Color = texture(ourTexture2, textureCoord);
	FragColor = mix(texColor, tex2Color, globalCol.a);

	vec3 lightDir = normalize(LightPos - vec3(viewSpacePos));

	vec3 diffuse = clamp(dot(lightDir, normal) +.1, 0.0, 1.0) * LightCol;

	FragColor = FragColor * vec4(diffuse,1);
	//FragColor = vec4(lightDir,1);
}