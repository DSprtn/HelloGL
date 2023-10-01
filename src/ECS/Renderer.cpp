#include "Renderer.h"

Renderer::Renderer(Entity* owner, std::string modelPath, Shader* shader) : Component(owner)
{
	model = AssetCache::LoadModel(modelPath);
	this->shader = shader;
}

void Renderer::Draw()
{
	shader->use();
	glm::mat4 t = m_Owner->Transform->WorldMatrix();
	glm::mat4 tNorm = glm::transpose(glm::inverse(t));

	shader->SetMat4(tNorm, "Normal");
	shader->SetMat4(t, "Model");

	model->Draw(*shader);
}

void Renderer::Start()
{
}

void Renderer::Update()
{
	Draw();
}
