#include "Transform.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <imgui.h>

Transform::Transform(Entity* owner) : Component(owner)
{
	Position = glm::vec3(0.0f);
	Rotation = glm::vec3(0.0f);
	Scale = glm::vec3(1.0f);
	CachedWorldMatrix = glm::mat4(1.0f);
	CachedLocalMatrix = glm::mat4(1.0f);
	Parent = nullptr;
}

void Transform::UpdateIMGUI()
{
	ImGui::Begin("Scene");
	ImGui::Begin(m_Owner->Name.c_str());
	bool changed = false;

	changed |= ImGui::InputFloat3("Position", &Position[0]);
	changed |= ImGui::InputFloat3("Rotation", &Rotation[0]);
	changed |= ImGui::InputFloat3("Scale", &Scale[0]);

	if (changed)
	{
		worldMatrixDirty = true;
		localMatrixDirty = true;
	}

	ImGui::End();
	ImGui::End();
}

glm::mat4 Transform::WorldMatrix()
{
	if (worldMatrixDirty)
	{
		Transform* p = Parent;

		glm::mat4 curr = CachedLocalMatrix;
		while (p != nullptr)
		{
			curr *= glm::inverse(p->LocalMatrix());
			p = p->Parent;
		}

		CachedWorldMatrix = curr;
		worldMatrixDirty = false;
	}

	return CachedWorldMatrix;
}

glm::mat4 Transform::LocalMatrix()
{
	if (localMatrixDirty)
	{
		//auto mat = glm::translate(glm::mat4(1.0f), Position);
		///mat = glm::scale(mat, Scale);
		//mat *= glm::quat(Rotation);
		//CachedLocalMatrix = mat;
		localMatrixDirty = false;
	}

	return CachedLocalMatrix;
}

void Transform::Start()
{
}
