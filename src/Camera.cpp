#include "Camera.h"


namespace {
	double lastCursorX = 0;
	double lastCursorY = 0;
	double lastMouseScrollY = 0;
	bool firstCursorPosReceived = false;
}


Camera::Camera(GLFWwindow* window)
{
	m_matrix = glm::mat4(1.0f);
	m_matrix = glm::translate(m_matrix, glm::vec3(0.0f, 0.0f, -3.0f));
	position = glm::vec3(0, 0, -3.0f);
	pitch = .0f;
	yaw = .0f;
	this->window = window;
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

	if (glfwGetKey(window, GLFW_KEY_W))
	{
		position += glm::vec3(0.0f, 0.0f, moveSpeed * deltaTime) * glm::quat(m_matrix);
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		position += glm::vec3(0.0f, 0.0f, -moveSpeed * deltaTime) * glm::quat(m_matrix);
	}
	if (glfwGetKey(window, GLFW_KEY_A))
	{
		position += glm::vec3(moveSpeed * deltaTime, 0.0f, 0.0f) * glm::quat(m_matrix);
	}
	if (glfwGetKey(window, GLFW_KEY_D))
	{
		position += glm::vec3(-moveSpeed * deltaTime, 0.0f, 0.0f) * glm::quat(m_matrix);
	}

	double cursorPosX, cursorPosY;
	glfwGetCursorPos(window, &cursorPosX, &cursorPosY);

	double xDelta = cursorPosX - lastCursorX;
	double yDelta = cursorPosY - lastCursorY;


	lastCursorX = cursorPosX;
	lastCursorY = cursorPosY;

	if (!firstCursorPosReceived)
	{
		firstCursorPosReceived = true;
		return;
	}

	pitch += yDelta * deltaTime;
	yaw += xDelta * deltaTime;
	pitch = glm::clamp(pitch, glm::radians(-90.0f), glm::radians(90.0f));
	m_matrix = glm::mat4(1.0f);
	
	m_matrix[3] = glm::highp_vec4(position.x, position.y, position.z, 1);
	auto rotation = glm::angleAxis(pitch, glm::vec3(1, 0, 0));
	rotation *= glm::angleAxis(yaw, glm::vec3(0, 1, 0));
	m_matrix = glm::mat4_cast(rotation) * m_matrix;
}
