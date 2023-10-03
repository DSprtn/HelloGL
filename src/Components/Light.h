#pragma once 

#include <Entity.h>
#include <Component.h>


class Light : public Component
{
	enum LightType
	{
		DirectionalLight,
		Spotlight,
		PointLight
	};

public:

	Light(Entity* owner, LightType type);
	~Light();

	LightType Type;

	glm::vec4 Position;
	glm::vec3 Color;
	float Intensity;
	float Range;

	glm::vec3 SpotlightDirection;
	float InnerCone;
	float OuterCone;



	// Inherited via Component
	virtual void Start() override;

	virtual void Update() override;

};