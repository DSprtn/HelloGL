#include "Light.h"
#include <Renderer.h>

Light::Light(Entity* owner, LightType type) : Component(owner)
{
	Color = glm::vec3(1, 1, 1);
	Position = glm::vec4(0.0f);
	InnerCone = 15.0f;
	OuterCone = 25.0f;
	Range = 5.0f;
	SpotlightDirection = glm::vec3(0, -1.0f, 0.0f);
	Type = type;
	Intensity = 1.0f;

	Renderer::Instance->RegisterLight(this);
}

Light::~Light()
{
	Renderer::Instance->UnregisterLight(this);
}

void Light::Start()
{
}

void Light::Update()
{

}
