#pragma once

#include <World.h>
#include <Input.h>
#include <Renderer.h>

class Engine
{
public:
	Engine();
	~Engine();

	static Engine* Instance;

	GLFWwindow* Window;
	Renderer Renderer;
	World* CurrentWorld;
	uint32_t FrameCount;
	bool IsRunning;

	void Init();
	void Shutdown();
	void Update();
	void LateUpdate();
	void OnRender();
	void HandleEvents();

protected:
	Input m_inputManager;

private:

	Engine(const Engine& that);
	Engine& operator=(const Engine& that);
};

