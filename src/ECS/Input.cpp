#include "Input.h"
#include <Engine.h>

Input* Input::Instance = nullptr;



Input::Input(GLFWwindow* window)
{
	Window = window;
	Instance = this;
}

Input::~Input()
{
	Instance = nullptr;
}

void Input::Init()
{
	glfwSetScrollCallback(Engine::Instance->Window, scroll_callback());
}

glm::vec2 Input::GetScroll()
{
	return glm::vec2();
}

void Input::scroll_callback(GLFWwindow* window, double x, double y)
{
}




