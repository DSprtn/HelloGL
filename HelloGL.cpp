#include <glad.h>
#include <glfw3.h>
#include <iostream>
#include <chrono>
#include <math.h>
#include "HelloGL.h"
#include "src/Shaders/Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "src/Camera.h"

namespace
{
	float uniformAlpha = .2f;
	double deltaTime = 0;
	float FOV = 75;
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
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

	if (key == GLFW_KEY_ESCAPE)
	{
		glfwSetWindowShouldClose(window, true);
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


	unsigned int diffuse = loadTexture("img/container2.png");
	unsigned int specular = loadTexture("img/container2_specular.png");
	unsigned int emissive = loadTexture("img/matrix.jpg");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuse);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specular);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, emissive);

	glUseProgram(defaultProgram);
	glUniform1i(glGetUniformLocation(defaultProgram, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(defaultProgram, "material.specular"), 1);
	glUniform1i(glGetUniformLocation(defaultProgram, "material.emissive"), 2);
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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
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


	Camera cam(window);

	while (!glfwWindowShouldClose(window))
	{
		computeDelta(lastMeasuredTime, totalElapsedTime);

		cam.Update(deltaTime);
		cam.SetProjection(FOV, 16.0 / 9.0, 0.1, 1000);


		glUseProgram(lightProgram);
		glm::mat4 model = glm::mat4(1.0f);
		glm::vec3 lightPos = glm::vec3(4 * cosf(totalElapsedTime * .1), 2 * sinf(totalElapsedTime * .1), -2 + 2 *cosf(totalElapsedTime * .1));

		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.1f));

		glUniformMatrix4fv(glGetUniformLocation(lightProgram, "Projection"), 1, GL_FALSE, glm::value_ptr(cam.ProjectionMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(lightProgram, "View"), 1, GL_FALSE, glm::value_ptr(cam.Matrix()));
		glUniformMatrix4fv(glGetUniformLocation(lightProgram, "Model"), 1, GL_FALSE, glm::value_ptr(model));


		glm::vec3 lightColor = glm::vec3(1.0f,0.0f,.0f);
		glUniform3fv(glGetUniformLocation(lightProgram, "LightCol"), 1, glm::value_ptr(lightColor));
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUseProgram(defaultProgram);
		
		glUniform3fv(glGetUniformLocation(defaultProgram, "light.position"), 1, glm::value_ptr(cam.Matrix()* glm::vec4(lightPos,1)));
		glUniform3fv(glGetUniformLocation(defaultProgram, "light.color"), 1, glm::value_ptr(lightColor));

		glUniform4f(glGetUniformLocation(defaultProgram, "globalCol"), (1 + static_cast<float>(sin(totalElapsedTime * 2))), 0.0f, 0.0f, uniformAlpha);

		glUniformMatrix4fv(glGetUniformLocation(defaultProgram, "Projection"), 1, GL_FALSE, glm::value_ptr(cam.ProjectionMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(defaultProgram, "View"), 1, GL_FALSE, glm::value_ptr(cam.Matrix()));
		

		std::cout << totalElapsedTime << std::endl;
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		for (int i = 0; i < 13; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			
			// model = glm::rotate(model, (float)totalElapsedTime * glm::radians(5.0f * i + 5.5f), glm::vec3(1.0f, .3f, 0.5f));

			 

			glm::mat4 normal = glm::transpose(glm::inverse(model));
			glUniformMatrix4fv(glGetUniformLocation(defaultProgram, "Normal"), 1, GL_FALSE, glm::value_ptr(normal));
			glUniformMatrix4fv(glGetUniformLocation(defaultProgram, "Model"), 1, GL_FALSE, glm::value_ptr(model));
			//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBindVertexArray(VAO);
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}



		glfwPollEvents();
		glfwSwapBuffers(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	glfwTerminate();

	return 0;
}


void computeDelta(std::chrono::steady_clock::time_point& last, double& totalElapsed)
{
	auto now = std::chrono::high_resolution_clock::now();
	const auto elapsed = std::chrono::high_resolution_clock::now() - last;
	const long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
	double currentElapsed = ((double)microseconds / 1000000.0);
	totalElapsed += currentElapsed;
	last = now;

	deltaTime = currentElapsed;
}


