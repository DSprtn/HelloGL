#include "Input.h"
#include "Input.h"
#include <Engine.h>

Input* Input::Instance = nullptr;

namespace
{
	void scroll_callback(GLFWwindow* window, double x, double y)
	{
		Input::Instance->ScrollInput = glm::vec2(x, y);
	}
}

Input::Input()
{
	Context = InputContext::Ingame;
	Instance = this;
	ScrollInput = glm::vec2(0.0f);

}

Input::~Input()
{
	Instance = nullptr;
}

void Input::Init()
{
	glfwSetScrollCallback(Engine::Instance->Window, scroll_callback);
}

void Input::Update()
{
	glfwPollEvents();

	if (GetKeyPressed(GLFW_KEY_ESCAPE))
	{
		Input::Instance->ToggleContext();
		if (Input::Instance->Context == Input::InputContext::Menus)
		{
			glfwSetInputMode(Engine::Instance->Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else
		{
			glfwSetInputMode(Engine::Instance->Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}
}

void Input::ToggleContext()
{
	if (Context == InputContext::Ingame)
	{
		Context = InputContext::Menus;
	}
	else 
	{
		Context = InputContext::Ingame;
	}
}

int Input::GetKeyPressed(int key)
{
	return glfwGetKey(Engine::Instance->Window, key);
}

int Input::GetKeyPressed(int key, InputContext filter)
{
	if (filter != Context)
	{
		return false;
	}
	return GetKeyPressed(key);
}

