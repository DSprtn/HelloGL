#include <Renderer.h>
#include <Input.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <Engine.h>
#include <numeric>
#include <Timer.h>

#include <tracy/TracyOpenGL.hpp>

#define LIGHT_COUNT 32

Renderer* Renderer::Instance = nullptr;

namespace
{
	bool VSync = true;

	std::array<Light::LightData, LIGHT_COUNT> lights;

	constexpr int fpsAvgCount = 30;
	std::array<double, 30> deltaTimeAverage{};
	int deltaIndex = 0;

	void SimInfoUpdate()
	{
		ImGui::Begin("Simulation");

		deltaTimeAverage[deltaIndex] = Core::Time::UnscaledDeltaTime;
		deltaIndex += 1;
		if (deltaIndex > deltaTimeAverage.size() - 1)
		{
			deltaIndex = 0;
		}

		double avg = std::accumulate(deltaTimeAverage.begin(), deltaTimeAverage.end(), 0.0f) / deltaTimeAverage.size();

		auto str = "FPS: " + std::to_string(1 / avg);
		auto frametimeStr = "Frametime: " + std::to_string(avg * 1000.0) + " ms";

		ImGui::Text(str.c_str());
		ImGui::Text(frametimeStr.c_str());
		ImGui::SliderFloat("Timescale", &Core::Time::Timescale, 0.0f, 20.0f);
		ImGui::End();
	}

	const std::string GPU_ZONE_NAME = "RenderFrame";
}

Renderer::Renderer()
{
	MainCamera = nullptr;
	assert(Instance == nullptr);
	Instance = this;
}

void Renderer::Init()
{
	TracyGpuContext;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glfwSwapInterval(VSync);
}

void Renderer::BeginFrame()
{
	TracyGpuZone("BeginFrame");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Renderer::Render()
{
	TracyGpuZone("Frame");

	SimInfoUpdate();

	ImGui::Begin("Renderer Settings");
	if (ImGui::Checkbox("VSync", &VSync))
	{
		glfwSwapInterval(VSync);
	}
	ImGui::End();

	if (MainCamera != nullptr)
	{
		{
			TracyGpuZone("Fill uniforms");
			lights.fill(Light::LightData());
			const int lightCount = std::min(static_cast<int>(Lights.size()), LIGHT_COUNT);

			for (int i = 0; i < lightCount; i++)
			{
				Light::LightData l;
				const Light& light = *Lights[i];

				l.color = light.Color;
				l.type = light.Type;
				l.position = MainCamera->ViewMatrix() * glm::vec4(light.m_Owner->Transform->GetWorldPosition(), 1.0f);
				l.direction = glm::normalize(MainCamera->ViewMatrix() * glm::vec4(light.m_Owner->Transform->GetWorldForward(), 0.0f));
				l.intensity = light.Intensity;
				l.range = light.Range;
				l.innerCone = glm::cos(glm::radians(light.InnerCone));
				l.outerCone = glm::cos(glm::radians(light.OuterCone));
				lights[i] = l;
			}

			for (Shader* s : Shaders)
			{
				s->use();
				s->SetMat4(MainCamera->ProjectionMatrix(), "Projection");
				s->SetMat4(MainCamera->ViewMatrix(), "View");

				for (int i = 0; i < lightCount; i++)
				{
					const std::string currLight = "lights[" + std::to_string(i) + "]";
					Light::LightData& l = lights[i];

					s->setVec3(l.color, currLight + ".color");
					s->setVec3(l.position, currLight + ".position");
					s->setVec3(l.direction, currLight + ".direction");
					s->setFloat(l.range, currLight + ".range");
					s->setFloat(l.intensity, currLight + ".intensity");
					s->setUniform1i(l.type, currLight + ".type");
					s->setFloat(l.innerCone, currLight + ".innerCone");
					s->setFloat(l.outerCone, currLight + ".outerCone");
					s->setUniform1i(lightCount, "LightCount");
				}
			}
		}
		for (MeshRenderer* m : Meshes)
		{
			TracyGpuZone("Draw Mesh");
			m->Draw();
		}
	}
}

void Renderer::EndFrame()
{
	{
		TracyGpuZone("Render ImGui");
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	{
		TracyGpuZone("Swap Buffers");
		glfwSwapBuffers(Engine::Instance->Window);
	}
	
	TracyGpuCollect;
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
