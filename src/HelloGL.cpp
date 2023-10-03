
#include <imgui.h>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <math.h>
#include "HelloGL.h"
#include "Shaders/Shader.h"
#include <algorithm>
#include <numeric>
#include <array>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include "Camera.h"

#include <Model.h>
#include <Entity.h>
#include <Renderer.h>
#include <World.h>

namespace
{
	float uniformAlpha = .2f;
	float FOV = 75;
}

bool locked = true;
bool shouldReloadShaders = false;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	FOV += yoffset * 10.0f;
	FOV = glm::clamp(FOV, 0.01f, 179.9f);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void kbCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		if (locked)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		locked = !locked;
	}

	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		if (locked)
		{
			shouldReloadShaders = true;
		}
	}

	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		uniformAlpha = std::clamp(uniformAlpha += 0.1f, 0.0f, 1.0f);
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		uniformAlpha = std::clamp(uniformAlpha -= 0.1f, 0.0f, 1.0f);
	}
}


int main(int argc, char* argv[])
{
	int resX = 1366;
	int resY = 768;

#pragma region init and init window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(resX, resY, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, resX, resY);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

#pragma endregion

	auto lastMeasuredTime = std::chrono::high_resolution_clock::now();
	double totalElapsedTime = 0.0;

	glfwSetKeyCallback(window, kbCallback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	float lightRadius[4] = {5,5,5,5};
	float pointLightColor[4][3]
	{
		{ 1.0f, 0.7f, .7f },
		{ 1.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f } 
	};
	float pointLightIntensity[4] = { 1,1,1,1 };

	float directionalLightColor[3]{ 0.05, 0.05, 0.25 };
	float directionalLightDirection[3]{ 0.0f, -1.0f, 0.4f };
	float directionalLightIntensity = 1.0f;

	float spotLightInnerCone = 12.5f;
	float spotLightOuterCone = 25.0f;
	float spotLightRange = 5.0f;
	float spotLightDirection[3]{ -0.3f, 0.3f, 0.56f };
	float spotLightPosition[3]{ -12.1f, 0.3f, 0.56f };
	float spotLightColor[3]{ 0.9, 0.1, 0.1 };
	float spotLightIntensity = 1.0f;
	bool  spotLightMountToHead = false;

	constexpr int fpsAvgCount = 30;
	std::array<float, 30> deltaTimeAverage{};
	int deltaIndex = 0;
	
	float moveDeltatimeMultiplier = 1.0f;
	
	double totalElapsedTimeByMoveDelta = 0.f;

	Camera cam(window);

	Shader defaultProgram = Shader("Shaders/Technicolor.vert", "Shaders/Technicolor.frag");
	Shader lightProgram = Shader("Shaders/Light.vert", "Shaders/Light.frag");


#pragma region CreateScene
	World world;


	auto createEntityWithModel = [&](std::string name, std::string model, Shader* shader)
	{
		auto e = world.CreateEntity<Entity>(name);
		e->AddComponent<Renderer>(model, shader);
		return e;
	};

	auto sponza = createEntityWithModel("Sponza", "assets/model/sponza/sponza.obj", &defaultProgram);

	Transform* parent = nullptr;
	for (int i = 0; i < 9; i++)
	{
		auto cubeChild = createEntityWithModel("Cube" + std::to_string(i), "assets/model/cube/cube.obj", &defaultProgram);
		cubeChild->Transform->SetParent(parent);
		cubeChild->Transform->SetLocalPosition(glm::vec3(0, 1.0f, 0));
		cubeChild->Transform->SetLocalScale(glm::vec3(.8f));
		parent = cubeChild->Transform;
	}
	
	sponza->Transform->SetLocalScale(glm::vec3(0.01f));

#pragma endregion

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glfwSwapInterval(0);

	while (!glfwWindowShouldClose(window))
	{
		if (shouldReloadShaders)
		{
			defaultProgram.Reload();
			lightProgram.Reload();
			shouldReloadShaders = false;

			glUseProgram(defaultProgram);
		}


		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		double deltaTime = computeDelta(lastMeasuredTime, totalElapsedTime);

		double moveDelta = deltaTime * moveDeltatimeMultiplier;
		totalElapsedTimeByMoveDelta += moveDelta;

		if (locked)
		{
			cam.Update(deltaTime);
		}
		else
		{
			cam.IgnoreNextUpdate();
		}
		
		cam.SetProjection(FOV, 16.0 / 9.0, 0.1, 1000);

#pragma region Setup_Lights

		ImGui::Begin("Light settings");

		const int numPointLights = 4;
		for (int i = 0; i < numPointLights; i++)
		{
			lightProgram.use();

			glm::mat4 model = glm::mat4(1.0f);
			glm::vec3 lightPos = glm::vec3(4 * cosf(totalElapsedTimeByMoveDelta * .1),4 + 4 * sinf(totalElapsedTimeByMoveDelta * .1), -2 + 4 * cosf(totalElapsedTimeByMoveDelta * .1));
			lightPos += glm::vec3((i + 1) * 3, 0, -(i + 1) * 2);
			lightPos -= glm::vec3(10, 0, -5);
			glm::vec3 lightPosViewspace = cam.Matrix() * glm::vec4(lightPos, 1);
			model = glm::translate(model, lightPos);
			model = glm::scale(model, glm::vec3(0.1f));
			glm::vec3 lightColor = glm::vec3(pointLightColor[i][0], pointLightColor[i][1], pointLightColor[i][2]);

			lightProgram.SetMat4(cam.ProjectionMatrix(), "Projection");
			lightProgram.SetMat4(cam.Matrix(), "View");
			lightProgram.SetMat4(model, "Model");
			lightProgram.setVec3(lightColor, "LightCol");



			defaultProgram.use();

			const std::string currLight = "pointLights[" + std::to_string(i) + "]";

			// Point light
			defaultProgram.setVec3(lightPosViewspace, currLight + ".position");
			defaultProgram.setVec3(lightColor, currLight + ".color");
			defaultProgram.setFloat(lightRadius[i], currLight + ".radius");
			defaultProgram.setFloat(pointLightIntensity[i], currLight + ".intensity");

			const std::string settingStr = "Lights.PointLight[" + std::to_string(i) + "]";

			if (ImGui::TreeNode(settingStr.c_str()))
			{
				ImGui::InputFloat("Intensity", &pointLightIntensity[i]);
				ImGui::SliderFloat("Radius", &lightRadius[i], 0.1f, 20.0f);
				ImGui::ColorEdit3("Color", &pointLightColor[i][0]);
				ImGui::TreePop();
			}
		}

		glm::vec3 directionalLightDir = cam.Matrix() * glm::normalize(glm::vec4(directionalLightDirection[0], directionalLightDirection[1], directionalLightDirection[2], 0.0f));
		glm::vec3 dirLightColor = glm::vec3(directionalLightColor[0], directionalLightColor[1], directionalLightColor[2]);

		glm::vec3 spotLightDir = cam.Matrix() * glm::normalize(glm::vec4(spotLightDirection[0], spotLightDirection[1], spotLightDirection[2], 0.0f));
		glm::vec3 spotLightPos = cam.Matrix() * glm::vec4(spotLightPosition[0], spotLightPosition[1], spotLightPosition[2], 1);
		glm::vec3 spotLightCol = glm::vec3(spotLightColor[0], spotLightColor[1], spotLightColor[2]);

		defaultProgram.use();

		// Directional light
		defaultProgram.setVec3(directionalLightDir, "directionalLight.direction");
		defaultProgram.setVec3(dirLightColor, "directionalLight.color");
		defaultProgram.setFloat(directionalLightIntensity, "directionalLight.intensity");

		// Spot light
		spotLightOuterCone = std::max(spotLightOuterCone, spotLightInnerCone);

		defaultProgram.setVec3(spotLightDir, "spotlight.direction");
		defaultProgram.setVec3(spotLightPos, "spotlight.position");

		if (spotLightMountToHead)
		{
			defaultProgram.setVec3(glm::vec3(0,0,-1), "spotlight.direction");
			defaultProgram.setVec3(glm::vec3(), "spotlight.position");
		}

		defaultProgram.setVec3(spotLightCol, "spotlight.color");
		defaultProgram.setFloat(spotLightRange, "spotlight.range");
		defaultProgram.setFloat(spotLightIntensity, "spotlight.intensity");
		defaultProgram.setFloat(glm::cos(glm::radians(spotLightInnerCone)), "spotlight.innerCone");
		defaultProgram.setFloat(glm::cos(glm::radians(spotLightOuterCone)), "spotlight.outerCone");

		glm::vec4 globalCol = glm::vec4(1 + static_cast<float>(sin(totalElapsedTimeByMoveDelta * 2)), 0.0f, 0.0f, uniformAlpha);

		defaultProgram.setVec4(globalCol, "globalCol");
		defaultProgram.SetMat4(cam.ProjectionMatrix(), "Projection");
		defaultProgram.SetMat4(cam.Matrix(), "View");

#pragma endregion



#pragma region ImGui

		if (ImGui::TreeNode("Lights.DirectionalLight"))
		{
			ImGui::InputFloat("Intensity", &directionalLightIntensity);
			ImGui::DragFloat3("Direction", &directionalLightDirection[0], 0.01f, -1.0f, 1.0f);
			ImGui::ColorEdit3("Color", &directionalLightColor[0]);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Lights.Spotlight"))
		{
			ImGui::InputFloat("Intensity", &spotLightIntensity);
			ImGui::SliderFloat("Range", &spotLightRange, 0.1f, 100.0f);
			ImGui::ColorEdit3("Color", &spotLightColor[0]);
			ImGui::SliderFloat("InnerCone", &spotLightInnerCone, 0.00f, 90.0f);
			ImGui::SliderFloat("OuterCone", &spotLightOuterCone, std::max(spotLightInnerCone, 0.0f), 90.0f);
			ImGui::DragFloat3("Position", &spotLightPosition[0], 0.05f, -25.0f, 25.0f);
			ImGui::DragFloat3("Direction", &spotLightDirection[0], 0.01f, -1.0f, 1.0f);
			ImGui::Checkbox("Mount to head", &spotLightMountToHead);
			ImGui::TreePop();
		}
		ImGui::End();

		ImGui::Begin("Simulation");
		

		deltaTimeAverage[deltaIndex] = deltaTime;
		deltaIndex += 1;
		if (deltaIndex > deltaTimeAverage.size() - 1)
		{
			deltaIndex = 0;
		}

		float avg = std::accumulate(deltaTimeAverage.begin(), deltaTimeAverage.end(), 0.0f) / deltaTimeAverage.size();


		auto str = "FPS: " + std::to_string(1 / avg);
		ImGui::Text(str.c_str());
		ImGui::SliderFloat("Timescale", &moveDeltatimeMultiplier, 0.0f, 20.0f);
		ImGui::End();

		world.Update();
		world.OnRender();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

#pragma endregion 

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

	return 0;
}


double computeDelta(std::chrono::steady_clock::time_point& last, double& totalElapsed)
{
	auto now = std::chrono::high_resolution_clock::now();
	const auto elapsed = std::chrono::high_resolution_clock::now() - last;
	const long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
	double currentElapsed = ((double)microseconds / 1000000.0);
	totalElapsed += currentElapsed;
	last = now;

	return currentElapsed;
}


