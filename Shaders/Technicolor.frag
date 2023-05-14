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
	float intensity;
};

struct DirectionalLight
{
	vec3 direction;
	vec3 color;
	float intensity;
};

struct Spotlight
{
	vec3 position;
	vec3 direction;
	vec3 color;
	float range;
	float outerCone;
	float innerCone;
	float intensity;
};

uniform PointLight light;
uniform DirectionalLight directionalLight;
uniform Spotlight spotlight;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
	sampler2D emissive;
    float shininess;
}; 
  
uniform Material material;


float attenuate(float dist, float lightRadius)
{
	return pow(max(1 - pow((dist/lightRadius),4),0),2);
}

vec3 getSpotLightContribution(Spotlight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 texDiffuse = vec3(texture(material.diffuse, textureCoord));
	vec3 texSpecular = vec3(texture(material.specular, textureCoord));
	vec3 emissive = vec3(texture(material.emissive, textureCoord));

	vec3 dirToLight = normalize(light.position - vec3(viewSpacePos));
	vec3 reflectDir = reflect(-dirToLight, normal);
	
	vec3 diffuse = clamp(dot(dirToLight, normal), 0.0, 1.0) * light.color;
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec3 specular = texSpecular * spec * light.color;

	vec3 ambient = light.color * .1;

	float d = length(light.position - vec3(viewSpacePos));
	float attenuation = attenuate(d,light.range);

	
	float spotdirToLightDot = dot(-light.direction, dirToLight);

	float epsilon   = light.innerCone - light.outerCone;
	float intensity = (spotdirToLightDot - light.outerCone) / epsilon;
	float coneContribution = clamp(intensity, 0.0, 1.0); 

	return vec3(light.intensity) * ( clamp(coneContribution,0,1) * attenuation * (texDiffuse * (diffuse + ambient) + specular));
}

vec3 getPointLightContribution(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 texDiffuse = vec3(texture(material.diffuse, textureCoord));
	vec3 texSpecular = vec3(texture(material.specular, textureCoord));
	vec3 emissive = vec3(texture(material.emissive, textureCoord));

	vec3 dirToLight = normalize(light.position - vec3(viewSpacePos));
	vec3 reflectDir = reflect(-dirToLight, normal);
	
	vec3 diffuse = clamp(dot(dirToLight, normal), 0.0, 1.0) * light.color;
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec3 specular = texSpecular * spec * light.color;

	vec3 ambient = light.color * .1;

	float d = length(light.position - vec3(viewSpacePos));
	float attenuation = attenuate(d,light.radius);

	return vec3(light.intensity) * (attenuation * (texDiffuse * (diffuse + ambient) + specular));
}

vec3 getDirectionalLightContribution(DirectionalLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 texSpecular = vec3(texture(material.specular, textureCoord));
	vec3 dirToLight = -light.direction;
	vec3 diffuse = clamp(dot(dirToLight, normal), 0.0, 1.0) * light.color;

	vec3 reflectDir = reflect(-dirToLight, normal);
	float specularStrength = 1.0f;

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);

	vec3 specular = texSpecular * spec * light.color;

	vec3 ambient = 0.01 * light.color;

	return vec3(light.intensity) * (vec3(texture(material.diffuse, textureCoord)) * diffuse + specular + ambient);
}

void main()
{
	vec3 viewDir = normalize(vec3(-viewSpacePos));
	vec3 col = getPointLightContribution(light, normal, vec3(viewSpacePos), viewDir);
	col += getDirectionalLightContribution(directionalLight, normal,vec3(viewSpacePos), viewDir);
	col += getSpotLightContribution(spotlight, normal, vec3(viewSpacePos), viewDir);
	
	FragColor = vec4(col, 1);
}