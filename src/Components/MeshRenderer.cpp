#include "MeshRenderer.h"
#include <Renderer.h>

MeshRenderer::MeshRenderer(Entity* owner, std::string modelPath, Shader* shader) : Component(owner)
{
	model = AssetCache::LoadModel(modelPath);
	this->shader = shader;
	Renderer::Instance->RegisterMeshRenderer(this);
}

MeshRenderer::~MeshRenderer()
{
	Renderer::Instance->UnregisterMeshRenderer(this);
}

void MeshRenderer::Draw()
{
	shader->use();
	glm::mat4 t = m_Owner->Transform->WorldMatrix();
	glm::mat4 tNorm = glm::transpose(glm::inverse(t));

	shader->SetMat4(tNorm, "Normal");
	shader->SetMat4(t, "Model");

	model->Draw(*shader);
}

void MeshRenderer::Start()
{
}

void MeshRenderer::Update()
{

}

void MeshRenderer::OnRender()
{
	//Draw();
}
