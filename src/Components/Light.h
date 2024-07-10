#pragma once 

#include <Entity.h>
#include <Component.h>


class Light : public Component
{
public:

	enum LightType
	{
		PointLight,
		Spotlight,
		DirectionalLight
	};

	struct LightData
	{
		glm::vec3 color;
		LightType type; // 0 = point, 1 = spot, 2 = directional
		glm::vec3 position;
		float range;
		glm::vec3 direction;
		float outerCone;
		float innerCone;
		float intensity;
	};

	Light(Entity* owner, LightType type);
	~Light();

	LightType Type;

	glm::vec3 Color;
	float Intensity;
	float Range;


	float InnerCone;
	float OuterCone;


	// Inherited via Component
	virtual void Start() override;

	virtual void OnRender() override;

protected:

	void UpdateImGui();

};