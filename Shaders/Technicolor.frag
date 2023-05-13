#version 330 core

out vec4 FragColor;
in vec3 vertexColor;
in vec2 textureCoord;
in vec3 normal;
in vec3 worldPos;
in vec4 viewSpacePos;

uniform vec4 globalCol;

struct PointLight
{
	vec3 position;
	vec3 color;
	float radius;
};

struct DirectionalLight
{
	vec3 direction;
	vec3 color;
};

struct SpotLight
{
	vec3 position;
	vec3 color;
	float outerAngle;
	float innerAngle;
};

uniform PointLight light;
uniform DirectionalLight directionalLight;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
	sampler2D emissive;
    float shininess;
}; 
  
uniform Material material;

vec3 getPointLightContribution(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 texSpecular = vec3(texture(material.specular, textureCoord));
	vec3 emissive = vec3(texture(material.emissive, textureCoord));

	vec3 lightDir = normalize(light.position - vec3(viewSpacePos));
	vec3 diffuse = clamp(dot(lightDir, normal), 0.0, 1.0) * light.color;


	vec3 reflectDir = reflect(-lightDir, normal);
	float specularStrength = 1.0f;

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);

	vec3 specular = (texSpecular + emissive * .5 * step(0.1, texSpecular)) * spec * (light.color + emissive * 2 * step(0.1, texSpecular));   

	vec3 ambient = light.color * .1;

	
	float d = length(light.position - vec3(viewSpacePos));

	//float attenuation = 1.0 / (1 + .22f * d + .2 * d * d);
	float attenuation = pow(min(1,light.radius / d), 2);

	return vec3(texture(material.diffuse, textureCoord)) * attenuation * (diffuse + ambient + specular);
}

vec3 getDirectionalLightContribution(DirectionalLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 texSpecular = vec3(texture(material.specular, textureCoord));
	vec3 lightDir = -light.direction;
	vec3 diffuse = clamp(dot(lightDir, normal), 0.0, 1.0) * light.color;

	vec3 reflectDir = reflect(-lightDir, normal);
	float specularStrength = 1.0f;

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);

	vec3 specular = texSpecular * spec * light.color;

	vec3 ambient = 0.01 * light.color;

	return vec3(texture(material.diffuse, textureCoord)) * diffuse + specular + ambient;
}

void main()
{
	vec3 viewDir = normalize(vec3(-viewSpacePos));
	FragColor = vec4(getPointLightContribution(light, normal, vec3(viewSpacePos), viewDir) + getDirectionalLightContribution(directionalLight, normal,vec3(viewSpacePos), viewDir), 1);
}