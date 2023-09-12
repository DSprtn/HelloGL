#version 330 core
#include "common.glsl"

out vec4 FragColor;

in VS_OUT {
	vec3 vertexColor;
	vec3 normal;
	vec2 textureCoord;
	vec3 worldPos;
	vec4 viewSpacePos;
	mat3 TBN;
} fs_in;

float attenuate(float dist, float lightRange)
{
	return pow(max(1 - pow((dist/lightRange),2),0),2);
}

vec3 getSpotLightContribution(Spotlight light, vec3 texDiffuse, vec3 texSpecular, vec3 normal, vec3 viewDir)
{
	vec3 dirToLight = normalize(light.position - vec3(fs_in.viewSpacePos));
	vec3 halfwayDir = normalize(viewDir + dirToLight);
	
	vec3 diffuse = clamp(dot(dirToLight, normal), 0.0, 1.0) * light.color * texDiffuse;
	vec3 specular = pow(max(dot(normal, halfwayDir), 0.0), 128) * texSpecular * light.color;

	vec3 ambient = texDiffuse * light.color * .1;

	float d = length(light.position - vec3(fs_in.viewSpacePos));

	float spotdirToLightDot = dot(-light.direction, dirToLight);

	float epsilon   = light.innerCone - light.outerCone;
	float intensity = (spotdirToLightDot - light.outerCone) / epsilon;
	float coneContribution = clamp(intensity, 0.0, 1.0); 

	return vec3(light.intensity) * (clamp(coneContribution,0,1) * attenuate(d,light.range) * (diffuse + ambient + specular));
}

vec3 getPointLightContribution(PointLight light, vec3 texDiffuse, vec3 texSpecular, vec3 normal, vec3 viewDir)
{
	vec3 dirToLight = normalize(light.position - vec3(fs_in.viewSpacePos));
	vec3 halfwayDir = normalize(dirToLight + viewDir);
	
	vec3 diffuse = clamp(dot(dirToLight, normal), 0.0, 1.0) * light.color * texDiffuse;
	vec3 specular = pow(max(dot(normal, halfwayDir), 0.0), 128) * texSpecular * light.color;

	vec3 ambient = texDiffuse * light.color * .05;

	float d = length(light.position - vec3(fs_in.viewSpacePos));

	return vec3(light.intensity) * attenuate(d,light.radius) * (diffuse + ambient + specular);
}

vec3 getDirectionalLightContribution(DirectionalLight light, vec3 texDiffuse, vec3 texSpecular, vec3 normal, vec3 viewDir)
{
	vec3 dirToLight = -light.direction;
	vec3 halfwayDir = normalize(dirToLight + viewDir);
	
	vec3 diffuse = clamp(dot(dirToLight, normal), 0.0, 1.0) * light.color * texDiffuse;
	vec3 specular = pow(max(dot(normal, halfwayDir), 0.0), 64) * texSpecular * light.color;

	vec3 ambient = texDiffuse * light.color * .1;

	return light.intensity * (diffuse + specular + ambient);
}

void main()
{
	vec4 texDiffuse = vec4(texture(material.diffuse, fs_in.textureCoord));
	vec3 texNormal = vec3(texture(material.normal, fs_in.textureCoord));
	vec3 texSpecular = vec3(texture(material.specular, fs_in.textureCoord));

	vec3 objNormal = fs_in.normal;

	if(texDiffuse.a < 0.0001)
	{
		discard;
	}

	vec3 viewDir = normalize(vec3(-fs_in.viewSpacePos));
	vec3 col = getDirectionalLightContribution(directionalLight, texDiffuse.rgb, texSpecular, objNormal, viewDir);

	for(int i = 0; i < POINT_LIGHT_COUNT; i++)
	{
		col += getPointLightContribution(pointLights[i], texDiffuse.rgb, texSpecular, objNormal, viewDir);
	}

	col += getSpotLightContribution(spotlight, texDiffuse.rgb, texSpecular, objNormal, viewDir);
	
	FragColor = vec4(col, 1);
}