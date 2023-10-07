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

	void kbCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
	{
		if (key < 0)
		{
			return; // invalid key 
		}
		assert(key < 500); // GLFW key indices should be below 500

		Input::Instance->keyPresses[key] = action == GLFW_PRESS;
	}
}

Input::Input()
{
	keyPresses = { 0 };
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
	glfwSetKeyCallback(Engine::Instance->Window, kbCallback);
	glfwSetInputMode(Engine::Instance->Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Input::Update()
{
	// Reset inputs
	ScrollInput = glm::vec2(0.0f);
	keyPresses.fill(0);

	glfwPollEvents();

	if (GetKeyDown(GLFW_KEY_ESCAPE))
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

bool Input::GetKeyDown(int key)
{
	return keyPresses[key];
}

bool Input::GetKeyDown(int key, InputContext filter)
{
	if (Context == filter)
	{
		return GetKeyDown(key);
	}
	return false;
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

