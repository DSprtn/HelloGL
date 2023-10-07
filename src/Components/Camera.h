#pragma once 

#include <Component.h>
#include <Entity.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

class Camera : public Component
{
public:

	Camera(Entity* owner);

	void LookAt(glm::vec3 position);

	void SetProjection(float FOV, float aspect, float near, float far);

	void Update(double delta);

	glm::mat4 ProjectionMatrix()
	{
		return m_projectionMatrix;
	}

	glm::mat4 Matrix()
	{
		return glm::inverse(m_matrix);
	}

	void IgnoreNextUpdate()
	{
		m_shouldIgnoreNextUpdate = true;
	}

private:
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_matrix;
	glm::vec3 position;
	glm::quat rotation;
	float pitch = 0;
	float yaw = 0;

	bool m_shouldIgnoreNextUpdate = true;


	// Inherited via Component
	virtual void Start() override;
	virtual void Update() override;

};