#include <Renderer.h>
#include <Input.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <Engine.h>

Renderer* Renderer::Instance = nullptr;

namespace
{
	bool VSync = true;
}

Renderer::Renderer()
{
	MainCamera = nullptr;
	assert(Instance == nullptr);
	Instance = this;
}

void Renderer::Init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Renderer::Render()
{
	ImGui::Begin("Renderer Settings");
	if (ImGui::Checkbox("VSync", &VSync))
	{
		glfwSwapInterval(VSync);
	}
	ImGui::End();

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

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	glfwSwapBuffers(Engine::Instance->Window);
}

void Renderer::Update()
{
	if (Input::Instance->GetKeyPressed(GLFW_KEY_R))
	{
		for (auto shader : Shaders)
		{
			shader->Reload();
		}
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

void Renderer::SetCamera(Camera* cam)
{
	this->MainCamera = cam;
}
