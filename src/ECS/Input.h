#pragma once 
#include <GLFW/glfw3.h>
#include <glm/ext/vector_float2.hpp>


class Input
{
public:
	static Input* Instance;

	Input(GLFWwindow* Window);
	~Input();
	

	void Init();

	glm::vec2 GetScroll();
	void GetKeyPressed();

private:
	glm::vec2 LastScrollInput;
	GLFWwindow* Window;

	void scroll_callback(GLFWwindow* window, double x, double y);

	Input(Input& other) = delete;
	Input(const Input&& other) = delete;

	Input& operator=(const Input& other) = delete;
	Input& operator=(Input&& other) = delete;
};