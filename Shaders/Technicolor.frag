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

	vec3 diffuse = clamp(dot(lightDir, normal), 0.0, 1.0) * LightCol;

	vec3 viewDir = normalize(vec3(-viewSpacePos));
	vec3 reflectDir = reflect(-lightDir, normal);

	float specularStrength = 1.0f;

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
	vec3 specular = specularStrength * spec * LightCol;  

	vec3 ambient = LightCol * .1;

	FragColor = FragColor * vec4(diffuse + specular + ambient,1);
	//FragColor = vec4(lightDir,1);
	//FragColor = vec4(specular,1);
	//FragColor = vec4(abs(viewDir),1);
}