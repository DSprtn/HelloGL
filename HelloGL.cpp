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

glm::mat4 camera = glm::mat4(1.0f);
float uniformAlpha = .2f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void kbCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) 
	{
		case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, true); break;
		case GLFW_KEY_UP:
			uniformAlpha = std::clamp(uniformAlpha += 0.1f, 0.0f, 1.0f);
			break;
		case GLFW_KEY_DOWN:
			uniformAlpha = std::clamp(uniformAlpha -= 0.1f, 0.0f, 1.0f);
			break;
		case GLFW_KEY_W:
			camera = glm::translate(camera, glm::vec3(0.0f, 0.0f, 0.1f));
			break;
		case GLFW_KEY_S:
			camera = glm::translate(camera, glm::vec3(0.0f, 0.0f, -0.1f));
			break;
	}
}

int main(int argc, char* argv[])
{

#pragma region init and init window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

#pragma endregion

	auto now = std::chrono::high_resolution_clock::now();
	auto last = std::chrono::high_resolution_clock::now();

	double totalElapsed = 0.0;

	auto defaultProgram = Shader("Shaders/Technicolor.vert", "Shaders/Technicolor.frag");

#pragma region create/bind meshes


	float tri[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	};

	float cube[] = {
			 0.5f, -0.5f, -0.5f, 0.0f, 0.0f,0.0f,  1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,0.0f,  0.0f, 0.0f,
			 0.5f,  0.5f, -0.5f, 0.0f, 0.0f,0.0f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f, 0.0f, 0.0f,0.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f, 0.0f, 0.0f,0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,0.0f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,0.0f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 0.0f, 0.0f,0.0f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f, 0.0f, 0.0f,0.0f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,0.0f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, 0.0f, 0.0f,0.0f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,0.0f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f, 0.0f, 0.0f,0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f, 0.0f, 0.0f,0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f, 0.0f, 0.0f,0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f, 0.0f, 0.0f,0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,0.0f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 0.0f, 0.0f,0.0f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f, 0.0f, 0.0f,0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,0.0f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,0.0f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,0.0f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,0.0f,  0.0f, 1.0f,
													 
			-0.5f,  0.5f, -0.5f, 0.0f, 0.0f,0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f, 0.0f, 0.0f,0.0f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f, 0.0f, 0.0f,0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 0.0f, 0.0f,0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, 0.0f, 0.0f,0.0f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	float quad[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};


	unsigned int triIndices[] =
	{
		0,1,2
	};

	unsigned int quadIndices[] =
	{
		0,1,2,
		0,2,3
	};

#pragma endregion

#pragma region Load textures

	stbi_set_flip_vertically_on_load(true);

	float borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, channels;
	auto* data = stbi_load("img/container.jpg", &width, &height, &channels, 0);

	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);


	auto* data2 = stbi_load("img/awesomeface.png", &width, &height, &channels, 0);

	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
	stbi_image_free(data2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glUseProgram(defaultProgram);
	glUniform1i(glGetUniformLocation(defaultProgram, "ourTexture"), 0);
	glUniform1i(glGetUniformLocation(defaultProgram, "ourTexture2"), 1);

#pragma endregion

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

	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

#pragma endregion


	glm::mat4 transform(1.0f);

	transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(0.0f,0.0f,1.0f));

	camera = glm::translate(camera, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projection = glm::perspective(glm::radians(70.0), 16.0 / 9.0, 0.1, 100.0);

	
	glfwSetKeyCallback(window, kbCallback);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		now = std::chrono::high_resolution_clock::now();
		const auto elapsed = std::chrono::high_resolution_clock::now() - last;
		const long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
		double currentElapsed = ((double)microseconds / 1000000.0);
		totalElapsed += currentElapsed;
		last = now;

		glUseProgram(defaultProgram);
		glUniform4f(glGetUniformLocation(defaultProgram, "globalCol"), (1 + static_cast<float>(sin(totalElapsed))) / 2.0f, 0.0f, 0.0f, uniformAlpha);

		glUniformMatrix4fv(glGetUniformLocation(defaultProgram, "Projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(defaultProgram, "View"), 1, GL_FALSE, glm::value_ptr(camera));
		
		std::cout << totalElapsed << std::endl;
		glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		for (int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, (float)totalElapsed * glm::radians(5.0f * i + 5.5f), glm::vec3(1.0f, .3f, 0.5f));

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


