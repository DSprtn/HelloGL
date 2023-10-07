#include <glad/glad.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

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

#include <Model.h>
#include <Entity.h>
#include <World.h>
#include <MeshRenderer.h>
#include <Renderer.h>
#include <Input.h>
#include <Engine.h>
#include <Camera.h>
#include <Timer.h>

namespace
{
	void computeTimings(std::chrono::steady_clock::time_point& last, float timeScale)
	{
		auto now = std::chrono::high_resolution_clock::now();
		const auto elapsed = std::chrono::high_resolution_clock::now() - last;
		const long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
		double currentElapsed = ((double)microseconds / 1000000.0);
		last = now;

		double deltaTime = currentElapsed;

		Timer::DeltaTime = deltaTime * timeScale;
		Timer::UnscaledDeltaTime = deltaTime;

		Timer::ElapsedTime += deltaTime * timeScale;
		Timer::UnscaledElapsedTime += deltaTime;
	}
}

int main(int argc, char* argv[])
{
	Engine engine;
	engine.Init();

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
	std::array<double, 30> deltaTimeAverage{};
	int deltaIndex = 0;
	
	float timeScale = 1.0f;
	GLFWwindow* window = engine.Window;

	Shader defaultProgram = Shader("Shaders/Technicolor.vert", "Shaders/Technicolor.frag");
	Shader lightProgram = Shader("Shaders/Light.vert", "Shaders/Light.frag");

#pragma region CreateScene

	auto createEntityWithModel = [&](std::string name, std::string model, Shader* shader)
	{
		auto e = engine.CurrentWorld->CreateEntity<Entity>(name);
		e->AddComponent<MeshRenderer>(model, shader);
		return e;
	};

	auto createEntity = [&](std::string name)
	{
		auto e = engine.CurrentWorld->CreateEntity<Entity>(name);
		return e;
	};

	auto camEntity = createEntity("MainCam");
	auto cam = camEntity->AddComponent<Camera>();

	auto sponza = createEntityWithModel("Sponza", "assets/model/sponza/sponza.obj", &defaultProgram);
	sponza->Transform->SetLocalScale(glm::vec3(0.01f));

	Transform* parent = nullptr;
	for (int i = 0; i < 9; i++)
	{
		auto cubeChild = createEntityWithModel("Cube" + std::to_string(i), "assets/model/cube/cube.obj", &defaultProgram);
		cubeChild->Transform->SetParent(parent);
		cubeChild->Transform->SetLocalPosition(glm::vec3(0, 1.0f, 0));
		cubeChild->Transform->SetLocalScale(glm::vec3(.8f));
		parent = cubeChild->Transform;
	}
	
#pragma endregion

	auto last = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		computeTimings(last, timeScale);

#pragma region Setup_Lights

		ImGui::Begin("Light settings");

		const int numPointLights = 4;
		for (int i = 0; i < numPointLights; i++)
		{
			lightProgram.use();

			glm::mat4 model = glm::mat4(1.0f);
			glm::vec3 lightPos = glm::vec3(4 * cosf(Timer::ElapsedTime * .1),4 + 4 * sinf(Timer::ElapsedTime * .1), -2 + 4 * cosf(Timer::ElapsedTime * .1));
			lightPos += glm::vec3((i + 1) * 3, 0, -(i + 1) * 2);
			lightPos -= glm::vec3(10, 0, -5);
			glm::vec3 lightPosViewspace = cam->ViewMatrix() * glm::vec4(lightPos, 1);
			model = glm::translate(model, lightPos);
			model = glm::scale(model, glm::vec3(0.1f));
			glm::vec3 lightColor = glm::vec3(pointLightColor[i][0], pointLightColor[i][1], pointLightColor[i][2]);

			lightProgram.SetMat4(cam->ProjectionMatrix(), "Projection");
			lightProgram.SetMat4(cam->ViewMatrix(), "View");
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

		glm::vec3 directionalLightDir = cam->ViewMatrix() * glm::normalize(glm::vec4(directionalLightDirection[0], directionalLightDirection[1], directionalLightDirection[2], 0.0f));
		glm::vec3 dirLightColor = glm::vec3(directionalLightColor[0], directionalLightColor[1], directionalLightColor[2]);

		glm::vec3 spotLightDir = cam->ViewMatrix() * glm::normalize(glm::vec4(spotLightDirection[0], spotLightDirection[1], spotLightDirection[2], 0.0f));
		glm::vec3 spotLightPos = cam->ViewMatrix() * glm::vec4(spotLightPosition[0], spotLightPosition[1], spotLightPosition[2], 1);
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

		//glm::vec4 globalCol = glm::vec4(1 + static_cast<float>(sin(totalElapsedTimeByMoveDelta * 2)), 0.0f, 0.0f, uniformAlpha);

		//defaultProgram.setVec4(globalCol, "globalCol");
		defaultProgram.SetMat4(cam->ProjectionMatrix(), "Projection");
		defaultProgram.SetMat4(cam->ViewMatrix(), "View");

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
		

		deltaTimeAverage[deltaIndex] = Timer::UnscaledDeltaTime;
		deltaIndex += 1;
		if (deltaIndex > deltaTimeAverage.size() - 1)
		{
			deltaIndex = 0;
		}

		double avg = std::accumulate(deltaTimeAverage.begin(), deltaTimeAverage.end(), 0.0f) / deltaTimeAverage.size();


		auto str = "FPS: " + std::to_string(1 / avg);
		auto frametimeStr = "Frametime: " + std::to_string(avg * 1000.0) + " ms";

		ImGui::Text(str.c_str());
		ImGui::Text(frametimeStr.c_str());
		ImGui::SliderFloat("Timescale", &timeScale, 0.0f, 20.0f);
		ImGui::End();
#pragma endregion 

		engine.Update();
		engine.LateUpdate();
		engine.OnRender();
	}


	return 0;
}




