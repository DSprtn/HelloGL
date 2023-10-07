#include "Camera.h"
#include <Input.h>

namespace {
	double lastCursorX = 0;
	double lastCursorY = 0;
	double lastMouseScrollY = 0;
}

Camera::Camera(Entity* owner) : Component(owner)
{
	m_matrix = glm::mat4(1.0f);
	m_matrix = glm::translate(m_matrix, glm::vec3(-11.0f, 1.6f, 0.46f));
	position = glm::vec3(-11.0f, 1.6f, 0.46f);
	pitch = .12f;
	yaw = 4.0f;
	SetProjection(70, 16.0f / 9.0f, 0.1, 100.0);
}

void Camera::LookAt(glm::vec3 targetPosition)
{
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	m_matrix = glm::lookAt(position, glm::vec3(0,0,-1) * glm::quat(m_matrix) + targetPosition, cameraUp);
}

void Camera::SetProjection(float FOV, float aspect, float near, float far)
{
	m_projectionMatrix = glm::perspective(glm::radians(FOV), aspect, near, far);
}

void Camera::Update(double deltaTime)
{
	bool fastMode = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);

	float moveSpeed = fastMode ? 24.0f : 4.0f;

	const auto& invMat = Matrix();

	if (Input::Instance->GetKeyPressed(GLFW_KEY_W, Input::InputContext::Ingame))
	{
		position += glm::vec3(0.0f, 0.0f, -moveSpeed * deltaTime) * glm::quat(invMat);
	}
	if (Input::Instance->GetKeyPressed(GLFW_KEY_S, Input::InputContext::Ingame))
	{
		position += glm::vec3(0.0f, 0.0f, moveSpeed * deltaTime) * glm::quat(invMat);
	}
	if (Input::Instance->GetKeyPressed(GLFW_KEY_A, Input::InputContext::Ingame))
	{
		position += glm::vec3(-moveSpeed * deltaTime, 0.0f, 0.0f) * glm::quat(invMat);
	}
	if (Input::Instance->GetKeyPressed(GLFW_KEY_D, Input::InputContext::Ingame))
	{
		position += glm::vec3(moveSpeed * deltaTime, 0.0f, 0.0f) * glm::quat(invMat);
	}

	double cursorPosX, cursorPosY;
	glfwGetCursorPos(window, &cursorPosX, &cursorPosY);

	double yDelta = cursorPosX - lastCursorX;
	double xDelta = cursorPosY - lastCursorY;

	lastCursorX = cursorPosX;
	lastCursorY = cursorPosY;

	if (m_shouldIgnoreNextUpdate)
	{
		m_shouldIgnoreNextUpdate = false;
		return;
	}

	pitch += xDelta * deltaTime;
	yaw += yDelta * deltaTime;
	pitch = glm::clamp(pitch, glm::radians(-90.0f), glm::radians(90.0f));

	m_matrix = glm::mat4(1.0f);
	auto rotation = glm::angleAxis(-yaw, glm::vec3(0, 1, 0));
	rotation *= glm::angleAxis(-pitch, glm::vec3(1, 0, 0));
	m_matrix = glm::mat4_cast(rotation) * m_matrix;
	m_matrix[3] = glm::highp_vec4(position.x, position.y, position.z, 1);

}

void Camera::Start()
{
}

void Camera::Update()
{
}
