#include "Light.h"
#include <Renderer.h>
#include <imgui.h>

Light::Light(Entity* owner, LightType type) : Component(owner)
{
	Color = glm::vec3(1, 1, 1);
	InnerCone = 15.0f;
	OuterCone = 25.0f;
	Range = 5.0f;
	Type = type;
	Intensity = 1.0f;

	Renderer::Instance->RegisterLight(this);
}

Light::~Light()
{
	Renderer::Instance->UnregisterLight(this);
}

void Light::Start()
{
}

void Light::OnRender()
{
	UpdateImGui();
}

void Light::UpdateImGui()
{
	ImGui::Begin("Light settings");
	
	if (ImGui::TreeNode(m_Owner->Name.c_str()))
	{
		const char* items[] = { "Point", "Spot", "Directional" };
		ImGui::Combo("combo", (int*)&Type, items, IM_ARRAYSIZE(items));

		ImGui::InputFloat("Intensity", &Intensity);
		if (Type != DirectionalLight)
		{
			ImGui::SliderFloat("Range", &Range, 0.1f, 20.0f);
		}
		ImGui::ColorEdit3("Color", &Color[0]);
		
		if (Type == Spotlight)
		{
			ImGui::SliderFloat("InnerCone", &InnerCone, 0.00f, 90.0f);
			ImGui::SliderFloat("OuterCone", &OuterCone, std::max(InnerCone, 0.0f), 90.0f);
		}

		ImGui::TreePop();
	}

	ImGui::End();
}
