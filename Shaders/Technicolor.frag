#version 330 core
#include "common.glsl"

out vec3 FragColor;

in VS_OUT {
	vec3 vertexColor;
	vec2 textureCoord;
	vec3 fragViewPos;
	mat3 TBN;
} fs_in;

float attenuate(float dist, float lightRange)
{
	return pow(max(1 - pow((dist/lightRange),2),0),2);
}

vec3 SpotLightContribution(Spotlight light, vec3 normal, vec3 viewDir, vec3 fragmentPosition, vec3 texDiffuse, vec3 texSpecular)
{
	vec3 dirToLight = normalize(light.position - fragmentPosition);
	vec3 halfwayDir = normalize(viewDir + dirToLight);
	
	vec3 diffuse = clamp(dot(dirToLight, normal), 0.0, 1.0) * light.color * texDiffuse.rgb;
	vec3 specular = pow(max(dot(normal, halfwayDir), 0.0), 128) * texSpecular * light.color;

	vec3 ambient = texDiffuse.rgb * light.color * .1;

	float dist = length(light.position - fragmentPosition);

	float spotdirToLightDot = dot(-light.direction, dirToLight);

	float epsilon   = light.innerCone - light.outerCone;
	float intensity = (spotdirToLightDot - light.outerCone) / epsilon;
	float coneContribution = clamp(intensity, 0.0, 1.0); 

	return vec3(light.intensity) * (clamp(coneContribution,0,1) * attenuate(dist,light.range) * (diffuse + ambient + specular));
}

vec3 getPointLightContribution(PointLight light, vec3 normal, vec3 viewDir, vec3 fragmentPosition, vec3 texDiffuse, vec3 texSpecular)
{
	vec3 dirToLight = normalize(light.position - fragmentPosition);
	vec3 halfwayDir = normalize(viewDir + dirToLight);

	
	vec3 diffuse = clamp(dot(dirToLight, normal), 0.0, 1.0) * light.color * texDiffuse;
	vec3 specular = pow(max(dot(normal, halfwayDir), 0.0), 128) * texSpecular * light.color;

	vec3 ambient = texDiffuse * light.color * .05;

	float dist = length(light.position - fragmentPosition);

	return vec3(light.intensity) * attenuate(dist,light.radius) * (diffuse + ambient + specular);
}

vec3 getDirectionalLightContribution(DirectionalLight light, vec3 normal, vec3 viewDir,vec3 texDiffuse, vec3 texSpecular)
{
	vec3 halfwayDir = normalize(viewDir + -light.direction);
	
	vec3 diffuse = clamp(dot(-light.direction, normal), 0.0, 1.0) * light.color * texDiffuse;
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

	vec3 texNormal = normalize(fs_in.TBN * normalize((vec3(texture(material.normal, fs_in.textureCoord)) * 2.0 - 1.0)));

	vec3 texSpecular = vec3(texture(material.specular, fs_in.textureCoord));

	vec3 viewDir = normalize(-fs_in.fragViewPos);

	vec3 col = getDirectionalLightContribution(directionalLight, texNormal, viewDir, texDiffuse.rgb, texSpecular);
	for(int i = 0; i < POINT_LIGHT_COUNT; i++)
	{
		col += getPointLightContribution(pointLights[i], texNormal, viewDir, fs_in.fragViewPos, texDiffuse.rgb, texSpecular);
	}
	col += SpotLightContribution(spotlight, texNormal, viewDir, fs_in.fragViewPos, texDiffuse.rgb, texSpecular);
	
	FragColor = col;
}