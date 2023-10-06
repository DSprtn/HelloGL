#include "Engine.h"
#include <iostream>
#include <exception>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>


namespace
{
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
}

Engine::Engine() {
	IsRunning = false;
	FrameCount = 0;
	Init();
}

Engine::~Engine() {
	Shutdown();
}

Engine* Engine::Instance = nullptr;

void Engine::Init()
{
	int resX = 1366;
	int resY = 768;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(resX, resY, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	glViewport(0, 0, resX, resY);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	IsRunning = true;
	Instance = this;
}

void Engine::Shutdown()
{
	delete CurrentWorld;
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	Instance = nullptr;
	std::cout << "Sayonara!" << std::endl;
}

void Engine::Update()
{
	if (CurrentWorld != nullptr) {
		CurrentWorld->Update();
	}
	m_physics.Update();
}

void Engine::LateUpdate()
{
	if (CurrentWorld != nullptr) {
		CurrentWorld->LateUpdate();
	}
}

void Engine::OnRender()
{
	SDL_RenderClear(Renderer);
	if (CurrentWorld != nullptr) {
		CurrentWorld->OnRender();
	}

	SDL_RenderPresent(Renderer);
}

void Engine::HandleEvents()
{
	SDL_Event sdlEvent;


	while (SDL_PollEvent(&sdlEvent)) {
		switch (sdlEvent.type) {
		case SDL_QUIT:
			IsRunning = false;
			break;
		default:
			break;
		}
	}

	m_inputManager.Update();
}


