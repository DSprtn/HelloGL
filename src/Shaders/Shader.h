#pragma once

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>
#include <filesystem>
#include <exception>

#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace
{
	static char const * const searchpaths[2] = {"./", "./Shaders/"};
	static char const * const includeShader[1] = {"/Shaders/common.glsl"};
	static bool gotCommon = false;
}

class Shader
{

public:

	Shader(const std::string& vertSourcePath, const std::string& fragSourcePath)
	{
		vertPath = vertSourcePath;
		fragPath = fragSourcePath;
		CompileProgram(vertSourcePath, fragSourcePath);
	}

	void Reload()
	{
		CompileProgram(vertPath, fragPath);
	}

	void SetMat4(const glm::mat4& mat4, const std::string& prop)
	{
		glUniformMatrix4fv(glGetUniformLocation(programID, prop.c_str()), 1, GL_FALSE, glm::value_ptr(mat4));
	}

	void setVec3(glm::vec3& vec, const std::string& prop)
	{
		glUniform3fv(glGetUniformLocation(programID, prop.c_str()), 1, glm::value_ptr(vec));
	}

	void setVec3(glm::vec3&& vec, const std::string& prop)
	{
		glUniform3fv(glGetUniformLocation(programID, prop.c_str()), 1, glm::value_ptr(vec));
	}

	void setVec4(glm::vec4& vec, const std::string& prop)
	{
		glUniform4fv(glGetUniformLocation(programID, prop.c_str()), 1, glm::value_ptr(vec));
	}

	void setFloat(float f, const std::string& prop)
	{
		glUniform1f(glGetUniformLocation(programID, prop.c_str()), f);
	}

	void setUniform1i(int i, const std::string& prop)
	{
		glUniform1i(glGetUniformLocation(programID, prop.c_str()), i);
	}

	void use()
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
			throw std::runtime_error("FILE DOES NOT EXIST");
		}
		std::ifstream t(sourcePath);

		t.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		std::stringstream buffer;
		buffer << t.rdbuf();
		std::string source = buffer.str();
		const char* sourceChar = source.c_str();

		glShaderSource(shader, 1, &sourceChar, NULL);

		for (const auto& path : includeShader)
		{
			std::string p(path);
			glNamedStringARB(GL_SHADER_INCLUDE_ARB, p.length(), path, source.length(), source.c_str());
		}

		glCompileShaderIncludeARB(shader, 1, includeShader, nullptr);
		

		int  success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			auto info = "ERROR::SHADER::COMPILATION_FAILED\n" + std::string(infoLog);
			std::cout << info;
			throw std::runtime_error(info.c_str());
		}
		
	}

	unsigned int programID;
	bool compiled = false;
	std::string vertPath;
	std::string fragPath;

};