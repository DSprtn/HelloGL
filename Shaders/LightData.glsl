struct LightData
{
	vec3 color;
	int type; // 0 = point, 1 = spot, 2 = directional
	vec3 position;
	float range;
	vec3 direction;
	float outerCone;
	float innerCone;
	float intensity;
};

#define LIGHT_COUNT 16

uniform LightData lights[LIGHT_COUNT];
uniform int LightCount;