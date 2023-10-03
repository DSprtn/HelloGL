#include "Transform.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <imgui.h>
#include <glm/gtc/quaternion.hpp>

Transform::Transform(Entity* owner) : Component(owner)
{
	Position = glm::vec3(0.0f);
	Rotation = glm::vec3(0.0f);
	Scale = glm::vec3(1.0f);
	CachedWorldMatrix = glm::mat4(1.0f);
	CachedLocalMatrix = glm::mat4(1.0f);
	Parent = nullptr;
}

void Transform::WalkNodes()
{
	if (ImGui::TreeNodeEx(m_Owner->Name.c_str(), ImGuiTreeNodeFlags_FramePadding))
	{
		bool changed = false;

		changed |= ImGui::DragFloat3("Position", &Position[0], 0.05f);
		changed |= ImGui::DragFloat3("Rotation", &Rotation[0], 0.05f);
		changed |= ImGui::DragFloat3("Scale", &Scale[0], 0.05f);

		if (changed)
		{
			MakeWorldMatrixDirty();
			localMatrixDirty = true;
		}
		for (Transform* child : Children)
		{
			child->WalkNodes();
		}
		ImGui::TreePop();
	}
}

void Transform::UpdateIMGUI()
{
	// Only build tree from root nodes
	if (Parent == nullptr)
	{
		ImGui::Begin("Scene");
		WalkNodes();
		ImGui::End();
	}
}

glm::mat4 Transform::WorldMatrix()
{
	if (worldMatrixDirty)
	{
		Transform* p = Parent;

		glm::mat4 curr = LocalMatrix();
		while (p != nullptr)
		{
			curr = p->LocalMatrix() * curr;
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
		auto mat = glm::translate(glm::mat4(1.0f), Position);
		mat *= glm::mat4_cast(glm::quat(Rotation));
		mat = glm::scale(mat, Scale);
		CachedLocalMatrix = mat;
		localMatrixDirty = false;
	}

	return CachedLocalMatrix;
}

void Transform::ChildAdded(Transform* t)
{
	Children.push_back(t);
}

void Transform::ChildRemoved(Transform* t)
{
	Children.erase(std::remove(Children.begin(), Children.end(), t), Children.end());
}

void Transform::MakeWorldMatrixDirty()
{
	worldMatrixDirty = true;
	for (Transform* child : Children)
	{
		child->MakeWorldMatrixDirty();
	}
}

Transform* Transform::GetRoot()
{
	Transform* root = this;
	Transform* parent = Parent;
	while (Parent != nullptr)
	{
		root = Parent;
		parent = root->Parent;
	}
	return root;
}

void Transform::Start()
{
}

void Transform::Update()
{
	UpdateIMGUI();
}
