#include <glad.h>
#include <glfw3.h>
#include <iostream>
#include <chrono>
#include <math.h>
#include "HelloGL.h"


const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 vecColor;\n"
"out vec3 vertexColor;"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"		vertexColor = vecColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core \n"
"out vec4 FragColor; \n"
"in vec3 vertexColor;"
"uniform vec4 globalCol;"
"void main()\n"
"{\n"
"FragColor = vec4(vertexColor,1.0f);\n"
"}\0";

void checkShaderLinkingCorrect(unsigned int  shaderProgram)
{
	int  success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
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

	float totalElapsed = 0.0f;


#pragma region createShaders

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	glCompileShader(vertexShader);

	CheckShaderCompilationSuccesful(vertexShader);

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	CheckShaderCompilationSuccesful(fragmentShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	checkShaderLinkingCorrect(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

#pragma endregion 


#pragma region create/bind meshes


	float tri[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	};


	unsigned int triIndices[] =
	{
		0,1,2
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tri), tri, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triIndices), triIndices, GL_STATIC_DRAW);

#pragma endregion

	while (!glfwWindowShouldClose(window))
	{
		now = std::chrono::high_resolution_clock::now();
		const auto elapsed = std::chrono::high_resolution_clock::now() - last;
		const unsigned long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
		totalElapsed += ((double)microseconds / 1000000);
		last = now;


		glUseProgram(shaderProgram);
		glUniform4f(glGetUniformLocation(shaderProgram, "globalCol"), (1 + sin(totalElapsed)) / 2, 0.0f, 0.0f, 1.0f);
		processInput(window);
		std::cout << totalElapsed << std::endl;
		glClearColor(0.0, 0.0, 0.2, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}

void CheckShaderCompilationSuccesful(unsigned int shader)
{
	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}


