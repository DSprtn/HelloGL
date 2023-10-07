#include "Camera.h"
#include <Input.h>
#include <Engine.h>
#include <Timer.h>

namespace {
	double lastCursorX = 0;
	double lastCursorY = 0;
}

Camera::Camera(Entity* owner) : Component(owner)
{
	SetProjection(70, 16.0f / 9.0f, 0.1, 100.0);
}

void Camera::SetProjection(float FOV, float aspect, float near, float far)
{
	m_projectionMatrix = glm::perspective(glm::radians(FOV), aspect, near, far);
}

void Camera::Start()
{
	m_Owner->Transform->SetLocalPosition(glm::vec3(-11.0f, 1.6f, 0.46f));
}

void Camera::Update()
{
	SetProjection(FOV, 16.0 / 9.0, 0.1, 1000);

	double cursorPosX, cursorPosY;
	glfwGetCursorPos(Engine::Instance->Window, &cursorPosX, &cursorPosY);

	double yDelta = cursorPosX - lastCursorX;
	double xDelta = cursorPosY - lastCursorY;

	lastCursorX = cursorPosX;
	lastCursorY = cursorPosY;

	if (Input::Instance->Context != Input::InputContext::Ingame)
	{
		return;
	}

	const double deltaTime = Timer::UnscaledDeltaTime;

	const bool fastMode = glfwGetKey(Engine::Instance->Window, GLFW_KEY_LEFT_SHIFT);

	const float moveSpeed = fastMode ? 24.0f : 4.0f;

	FOV += Input::Instance->ScrollInput.y * 500.0f * deltaTime;
	FOV = glm::clamp(FOV, 0.01f, 179.9f);

	const auto& invMatQuat = glm::quat(ViewMatrix());

	if (Input::Instance->GetKeyPressed(GLFW_KEY_W, Input::InputContext::Ingame))
	{
		m_Owner->Transform->Translate(glm::vec3(0.0f, 0.0f, -moveSpeed * deltaTime) * invMatQuat);
	}
	if (Input::Instance->GetKeyPressed(GLFW_KEY_S, Input::InputContext::Ingame))
	{
		m_Owner->Transform->Translate(glm::vec3(0.0f, 0.0f, moveSpeed * deltaTime) * invMatQuat);
	}
	if (Input::Instance->GetKeyPressed(GLFW_KEY_A, Input::InputContext::Ingame))
	{
		m_Owner->Transform->Translate(glm::vec3(-moveSpeed * deltaTime, 0.0f, 0.0f) * invMatQuat);
	}
	if (Input::Instance->GetKeyPressed(GLFW_KEY_D, Input::InputContext::Ingame))
	{
		m_Owner->Transform->Translate(glm::vec3(moveSpeed * deltaTime, 0.0f, 0.0f) * invMatQuat);
	}

	const float lookSensitivity = 15.0f;

	glm::vec3 rotation = m_Owner->Transform->GetLocalRotation();

	float pitch = rotation.x + -xDelta * lookSensitivity * deltaTime;
	pitch = glm::clamp(pitch, -90.0f, 90.0f);

	const float yaw = rotation.y + -yDelta * lookSensitivity * deltaTime;

	rotation = glm::vec3(pitch, yaw, 0.0f);

	m_Owner->Transform->SetLocalRotation(rotation);
}