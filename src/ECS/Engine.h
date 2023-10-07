#pragma once

#include <glad/glad.h>
#include <Renderer.h>
#include <World.h>
#include <Input.h>


class Engine
{
public:
	Engine();
	~Engine();

	static Engine* Instance;

	GLFWwindow* Window;

	World* CurrentWorld;
	uint32_t FrameCount;
	bool IsRunning;

	void Init();
	void Shutdown();
	void Update();
	void LateUpdate();
	void OnRender();

protected:
	Input* Input;
	Renderer* Renderer;

private:

	Engine(const Engine& that);
	Engine& operator=(const Engine& that);
};

