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

struct Material {
    sampler2D diffuse;
    sampler2D specular;
	sampler2D emissive;
	sampler2D normal;
    float shininess;
};

#define POINT_LIGHT_COUNT 4
#define SPOTLIGHT_COUNT 4

uniform Material material;

uniform PointLight pointLights[POINT_LIGHT_COUNT];
uniform DirectionalLight directionalLight;
uniform Spotlight spotlight;

uniform vec4 globalCol;