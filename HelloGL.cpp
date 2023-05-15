﻿#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <math.h>
#include "HelloGL.h"
#include "src/Shaders/Shader.h"
#include <algorithm>
#include <numeric>
#include <array>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"
#include "src/Camera.h"

namespace
{
	float uniformAlpha = .2f;
	float FOV = 75;
}

bool locked = true;
bool shouldReloadShaders = false;

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path, bool sRGB = false)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format = GL_RED;
		
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;
		
		GLenum internalFormat = format;
		
		if (sRGB)
		{
			if (nrComponents == 3)
			{
				internalFormat = GL_SRGB;
			}
			else if (nrComponents == 4)
			{
				internalFormat = GL_SRGB_ALPHA;
			}
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

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
		//glfwSetWindowShouldClose(window, true);
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


	auto defaultProgram = Shader("Shaders/Technicolor.vert", "Shaders/Technicolor.frag");
	auto lightProgram = Shader("Shaders/Light.vert", "Shaders/Light.frag");

#pragma region Load textures


	unsigned int diffuse = loadTexture("img/container2.png", true);
	unsigned int specular = loadTexture("img/container2_specular.png");
	unsigned int emissive = loadTexture("img/matrix.jpg", true);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuse);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specular);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, emissive);

	glUseProgram(defaultProgram);
	defaultProgram.setUniform1i(0, "material.diffuse");
	defaultProgram.setUniform1i(1, "material.specular");
	defaultProgram.setUniform1i(2, "material.emissive");


#pragma endregion

#pragma region create/bind meshes


	float cube[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f


	};


	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  5.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f),
		glm::vec3(0,10,0),
		glm::vec3(10,0,0),
		glm::vec3(0,0,10)
	};


	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	// Light Cube
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


#pragma endregion

	auto lastMeasuredTime = std::chrono::high_resolution_clock::now();
	double totalElapsedTime = 0.0;

	glfwSetKeyCallback(window, kbCallback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);



	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	Camera cam(window);


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
	float spotLightDirection[3]{ 0.0f, -1.0f, 0.5f };
	float spotLightPosition[3]{ 0.0f, 3.0f, 0.0f };
	float spotLightColor[3]{ 0.9, 0.1, 0.1 };
	float spotLightIntensity = 1.0f;
	bool  spotLightMountToHead = false;

	
	constexpr int fpsAvgCount = 30;
	std::array<float, 30> deltaTimeAverage{};
	int deltaIndex = 0;
	

	float moveDeltatimeMultiplier = 1.0f;
	
	double totalElapsedTimeByMoveDelta = 0.f;

	while (!glfwWindowShouldClose(window))
	{
		if (shouldReloadShaders)
		{
			defaultProgram.Reload();
			lightProgram.Reload();
			shouldReloadShaders = false;

			glUseProgram(defaultProgram);
			defaultProgram.setUniform1i(0, "material.diffuse");
			defaultProgram.setUniform1i(1, "material.specular");
			defaultProgram.setUniform1i(2, "material.emissive");
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
			glm::vec3 lightPos = glm::vec3(4 * cosf(totalElapsedTimeByMoveDelta * .1),4 * sinf(totalElapsedTimeByMoveDelta * .1), -2 + 4 * cosf(totalElapsedTimeByMoveDelta * .1));
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

			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);

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

#pragma endregion

#pragma region drawMeshes
		defaultProgram.SetMat4(cam.ProjectionMatrix(), "Projection");
		defaultProgram.SetMat4(cam.Matrix(), "View");

		for (int i = 0; i < 13; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			
			model = glm::rotate(model, (float)totalElapsedTimeByMoveDelta * glm::radians(5.0f * i + 5.5f), glm::vec3(1.0f, .3f, 0.5f));

			glm::mat4 normal = glm::transpose(glm::inverse(model));
			defaultProgram.SetMat4(normal, "Normal");
			defaultProgram.SetMat4(model, "Model");

			glEnable(GL_FRAMEBUFFER_SRGB);

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glm::mat4 floorModel = glm::mat4(1.0f);
		floorModel = glm::translate(floorModel, glm::vec3(0, -55.0f, 0));
		floorModel = glm::scale(floorModel, glm::vec3(100.0f));

		glm::mat4 normal = glm::transpose(glm::inverse(floorModel));
		defaultProgram.SetMat4(normal, "Normal");
		defaultProgram.SetMat4(floorModel, "Model");
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

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
		ImGui::SliderFloat("Delta time", &moveDeltatimeMultiplier, 0.0f, 20.0f);
		ImGui::End();

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


