#version 330 core
#include "common.glsl"

out vec3 FragColor;

in VS_OUT {
	vec3 vertexColor;
	vec2 textureCoord;
	vec3 worldPos;
	vec3 fragTangentPos;
	vec3 fragViewPos;
	mat3 TBN;
	vec3 pointLightPositions[POINT_LIGHT_COUNT];
	vec3 directionalLightDir;
	vec3 spotlightPos;
	vec3 spotLightDir;
} fs_in;

float attenuate(float dist, float lightRange)
{
	return pow(max(1 - pow((dist/lightRange),2),0),2);
}

vec3 getSpotLightContribution(Spotlight light, vec3 pos, vec3 lightDir, vec3 texDiffuse, vec3 texSpecular, vec3 normal, vec3 viewDir)
{
	vec3 dirToLight = normalize(pos - fs_in.fragTangentPos);
	vec3 halfwayDir = normalize(dirToLight + viewDir);
	
	vec3 diffuse = clamp(dot(dirToLight, normal), 0.0, 1.0) * light.color * texDiffuse;
	vec3 specular = pow(max(dot(normal, halfwayDir), 0.0), 128) * texSpecular * light.color;

	vec3 ambient = texDiffuse * light.color * .1;

	float d = length(pos - fs_in.fragTangentPos);
	float d2 = length(light.position -fs_in.fragViewPos);
	float d3 = length((fs_in.TBN * light.position) - (fs_in.TBN * fs_in.fragViewPos));

	float spotdirToLightDot = dot(normalize(fs_in.fragViewPos - light.position), light.direction);

	float epsilon   = light.innerCone - light.outerCone;
	float intensity = (spotdirToLightDot - light.outerCone) / epsilon;
	float coneContribution = clamp(intensity, 0.0, 1.0); 


	return vec3(d /5);

	return vec3(light.intensity) * (clamp(coneContribution,0,1) * attenuate(d,light.range) * (diffuse + ambient + specular));
}

vec3 getPointLightContribution(PointLight light, vec3 pos, vec3 texDiffuse, vec3 texSpecular, vec3 normal, vec3 viewDir)
{
	vec3 dirToLight = normalize(pos - vec3(fs_in.fragTangentPos));
	vec3 halfwayDir = normalize(dirToLight + viewDir);
	
	vec3 diffuse = clamp(dot(dirToLight, normal), 0.0, 1.0) * light.color * texDiffuse;
	vec3 specular = pow(max(dot(normal, halfwayDir), 0.0), 128) * texSpecular * light.color;

	vec3 ambient = texDiffuse * light.color * .05;

	float d = length(light.position - fs_in.fragViewPos);

	return vec3(light.intensity) * attenuate(d,light.radius) * (diffuse + ambient + specular);
}

vec3 getDirectionalLightContribution(DirectionalLight light, vec3 lightDir, vec3 texDiffuse, vec3 texSpecular, vec3 normal, vec3 viewDir)
{
	vec3 halfwayDir = normalize(lightDir + viewDir);
	
	vec3 diffuse = clamp(dot(lightDir, normal), 0.0, 1.0) * light.color * texDiffuse;
	vec3 specular = pow(max(dot(normal, halfwayDir), 0.0), 64) * light.color * texSpecular;

	vec3 ambient = texDiffuse * light.color * .1;

	return light.intensity * (diffuse + specular + ambient);
}

void main()
{
	vec4 texDiffuse = vec4(texture(material.diffuse, fs_in.textureCoord));

	if(texDiffuse.a < 0.0001)
	{
		discard;
	}

	vec3 texNormal = normalize(vec3(texture(material.normal, fs_in.textureCoord)) * 2.0 - 1.0);

	vec3 texSpecular = vec3(texture(material.specular, fs_in.textureCoord));

	vec3 viewDir = normalize(-fs_in.fragTangentPos);
	vec3 col = vec3(0);//getDirectionalLightContribution(directionalLight, normalize(-fs_in.directionalLightDir), texDiffuse.rgb, texSpecular, texNormal, viewDir);

	for(int i = 0; i < POINT_LIGHT_COUNT; i++)
	{
		col += getPointLightContribution(pointLights[i], fs_in.pointLightPositions[i], texDiffuse.rgb, texSpecular, texNormal, viewDir);
	}

	col += getSpotLightContribution(spotlight, fs_in.spotlightPos, normalize(fs_in.spotLightDir), texDiffuse.rgb, texSpecular, texNormal, viewDir);
	
	FragColor = col;

}