#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>
#include <filesystem>
#include <exception>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Shader
{

public:

	Shader(const std::string& vertSourcePath, const std::string& fragSourcePath)
	{
		vertPath = vertSourcePath;
		fragPath = fragSourcePath;
		CompileProgram(vertSourcePath, fragSourcePath);
	}

	void Shader::Reload()
	{
		CompileProgram(vertPath, fragPath);
	}

	void Shader::SetMat4(const glm::mat4& mat4, const std::string& prop)
	{
		glUniformMatrix4fv(glGetUniformLocation(programID, prop.c_str()), 1, GL_FALSE, glm::value_ptr(mat4));
	}

	void Shader::setVec3(glm::vec3& vec, const std::string& prop)
	{
		glUniform3fv(glGetUniformLocation(programID, prop.c_str()), 1, glm::value_ptr(vec));
	}

	void Shader::setVec4(glm::vec4& vec, const std::string& prop)
	{
		glUniform4fv(glGetUniformLocation(programID, prop.c_str()), 1, glm::value_ptr(vec));
	}

	void Shader::setFloat(float f, const std::string& prop)
	{
		glUniform1f(glGetUniformLocation(programID, prop.c_str()), f);
	}

	void Shader::setUniform1i(int i, const std::string& prop)
	{
		glUniform1i(glGetUniformLocation(programID, prop.c_str()), i);
	}

	void Shader::use()
	{
		glUseProgram(programID);
	}

	operator int() const { return programID; }


private:

	void CompileProgram(const std::string& vertSourcePath, const std::string& fragSourcePath)
	{
		auto newProgramID = glCreateProgram();

		unsigned int vert = glCreateShader(GL_VERTEX_SHADER);
		unsigned int frag = glCreateShader(GL_FRAGMENT_SHADER);

		CompileShader(vertSourcePath, vert);
		CompileShader(fragSourcePath, frag);

		glAttachShader(newProgramID, vert);
		glAttachShader(newProgramID, frag);
		glLinkProgram(newProgramID);

		int  success;
		char infoLog[512];
		glGetProgramiv(newProgramID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(newProgramID, 512, NULL, infoLog);
		}

		glDeleteShader(frag);
		glDeleteShader(vert);

		if (success)
		{
			if (compiled)
			{
				glDeleteProgram(this->programID);
			}
			this->programID = newProgramID;
			compiled = true;
		}
		
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

	unsigned int programID;
	bool compiled = false;
	std::string vertPath;
	std::string fragPath;

};

#endif 