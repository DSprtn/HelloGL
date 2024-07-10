#pragma once 

#include <Entity.h>
#include <Component.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Camera : public Component
{
public:

	Camera(Entity* owner);

	glm::mat4 ProjectionMatrix()
	{
		return m_projectionMatrix;
	}

	glm::mat4 ViewMatrix()
	{
		return glm::inverse(m_Owner->Transform->WorldMatrix());
	}

private:
	glm::mat4 m_projectionMatrix;
	float FOV = 75;

	void SetProjection(float FOV, float aspect, float near, float far);

	// Inherited via Component
	virtual void Start() override;
	virtual void Update() override;

};