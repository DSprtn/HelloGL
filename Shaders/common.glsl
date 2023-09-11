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

#define POINT_LIGHT_COUNT 4

uniform PointLight pointLights[POINT_LIGHT_COUNT];
uniform DirectionalLight directionalLight;
uniform Spotlight spotlight;