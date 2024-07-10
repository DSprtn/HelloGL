#include "Shader.h"
#include <Renderer.h>

Shader::Shader(const std::string& vertSourcePath, const std::string& fragSourcePath)
{
	vertPath = vertSourcePath;
	fragPath = fragSourcePath;
	CompileProgram(vertSourcePath, fragSourcePath);
	Renderer::Instance->RegisterShader(this);
}

Shader::~Shader()
{
	Renderer::Instance->UnregisterShader(this);
}