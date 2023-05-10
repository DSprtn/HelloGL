#version 330 core

out vec4 FragColor;
in vec3 vertexColor;
in vec2 textureCoord;
in vec3 normal;
in vec3 worldPos;
in vec4 viewSpacePos;

uniform vec4 globalCol;

struct Light
{
	vec3 position;
	vec3 color;
};

uniform Light light;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
	sampler2D emissive;
    float shininess;
}; 
  
uniform Material material;

void main()
{
	vec3 texColor = vec3(texture(material.diffuse, textureCoord));
	vec3 texSpecular = vec3(texture(material.specular, textureCoord));
	vec3 emissive = vec3(texture(material.emissive, textureCoord));

	vec3 lightDir = normalize(light.position - vec3(viewSpacePos));

	vec3 diffuse = clamp(dot(lightDir, normal), 0.0, 1.0) * light.color * texColor;

	vec3 viewDir = normalize(vec3(-viewSpacePos));
	vec3 reflectDir = reflect(-lightDir, normal);

	float specularStrength = 1.0f;

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec3 specular = texSpecular * spec * light.color;  

	vec3 ambient = light.color * .1 * texColor;

	FragColor = vec4(emissive * globalCol.r + diffuse + specular + ambient,1);
	//FragColor = vec4(lightDir,1);
	//FragColor = vec4(specular,1);
	//FragColor = vec4(abs(viewDir),1);
	//FragColor = vec4(texSpecular,1);
	//FragColor = texture(material.specular, textureCoord);
}