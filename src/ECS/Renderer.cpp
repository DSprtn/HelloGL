#include "Renderer.h"

void Renderer::Draw(Shader& shader)
{
	shader.use();
	glm::mat4 t = m_Owner->Transform.WorldMatrix();
	glm::mat4 tNorm = glm::transpose(glm::inverse(t));

	shader.SetMat4(tNorm, "Normal");
	shader.SetMat4(t, "Model");

	model.Draw(shader);
}

void Renderer::Start()
{
}
