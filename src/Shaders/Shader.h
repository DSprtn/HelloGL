#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glad.h>
#include <glfw3.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>
#include <filesystem>
#include <exception>

class Shader
{

public:

	Shader(const std::string& vertSourcePath, const std::string& fragSourcePath)
	{
		unsigned int vert = glCreateShader(GL_VERTEX_SHADER);
		unsigned int frag = glCreateShader(GL_FRAGMENT_SHADER);

		CompileShader(vertSourcePath, vert);
		CompileShader(fragSourcePath, frag);


		programID = glCreateProgram();

		glAttachShader(programID, vert);
		glAttachShader(programID, frag);
		glLinkProgram(programID);

		int  success;
		char infoLog[512];
		glGetProgramiv(programID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(programID, 512, NULL, infoLog);
		}

		glDeleteShader(frag);
		glDeleteShader(vert);
	}

	void CompileShader(const std::string& sourcePath, const unsigned int shader)
	{
		if (!std::filesystem::exists(sourcePath))
		{
			throw std::exception("FILE DOES NOT EXIST");
		}
		std::ifstream t(sourcePath);

		t.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	
		std::stringstream buffer;
		buffer << t.rdbuf();
		std::string source = buffer.str();
		const char* sourceChar = source.c_str();

		glShaderSource(shader, 1, &sourceChar, NULL);

		glCompileShader(shader);

		int  success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			auto info = "ERROR::SHADER::COMPILATION_FAILED\n" + std::string(infoLog);
			std::cout << info;
			throw std::exception(info.c_str());
		}
	}

	operator int() const { return programID; }


private:

	unsigned int programID;

};

#endif 