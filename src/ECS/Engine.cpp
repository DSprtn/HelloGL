#include "Engine.h"
#include <iostream>
#include <exception>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
#include <Timer.h>
#include <numeric>

namespace
{
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

}

Engine::Engine() {
	Instance = this;
	IsRunning = false;
	FrameCount = 0;

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

	Window = glfwCreateWindow(resX, resY, "LearnOpenGL", NULL, NULL);
	if (Window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(Window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	glViewport(0, 0, resX, resY);

	glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);

	// Init IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(Window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	Instance = this;
	CurrentWorld = new World();
	Renderer = new class Renderer();
	Input = new class Input();
	Renderer->Init();
	Input->Init();
	IsRunning = true;
}

void Engine::Update()
{
	Input->Update();
	Renderer->Update();

	if (CurrentWorld != nullptr) {
		CurrentWorld->Update();
	}
}

void Engine::LateUpdate()
{
	if (CurrentWorld != nullptr) {
		CurrentWorld->LateUpdate();
	}
}

void Engine::OnRender()
{
	Renderer->BeginFrame();

	Renderer->Render();

	if (CurrentWorld != nullptr) {
		CurrentWorld->OnRender();
	}

	Renderer->EndFrame();

	FrameCount++;
}

void Engine::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	delete CurrentWorld;
	glfwTerminate();
	Instance = nullptr;
	std::cout << "Sayonara!" << std::endl;
}