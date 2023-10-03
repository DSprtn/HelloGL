#include <Renderer.h>

Renderer* Renderer::Instance = nullptr;

Renderer::Renderer()
{
	assert(Instance == nullptr);
	Instance = this;
}

void Renderer::Render()
{
	for (Shader* s : Shaders)
	{
		for (Light* l : Lights)
		{
			
		}
	}

	for (MeshRenderer* m : Meshes)
	{
		m->Draw();
	}
}

void Renderer::RegisterShader(Shader* s)
{
	Shaders.push_back(s);
}

void Renderer::UnregisterShader(Shader* s)
{
	Shaders.erase(std::remove(Shaders.begin(), Shaders.end(), s), Shaders.end());
}

void Renderer::RegisterMeshRenderer(MeshRenderer* m)
{
	Meshes.push_back(m);
}

void Renderer::UnregisterMeshRenderer(MeshRenderer* m)
{
	Meshes.erase(std::remove(Meshes.begin(), Meshes.end(), m), Meshes.end());
}

void Renderer::RegisterLight(Light* light)
{
	Lights.push_back(light);
}

void Renderer::UnregisterLight(Light* light)
{
	Lights.erase(std::remove(Lights.begin(), Lights.end(), light), Lights.end());
}
