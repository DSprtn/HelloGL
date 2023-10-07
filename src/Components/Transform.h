#pragma once 

#include <Component.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <vector>
#include <Entity.h>

class Transform : public Component
{
public:

	Transform(Entity* owner);

	Transform* GetParent()
	{
		return Parent;
	}

	bool HasParent()
	{
		return Parent != nullptr;
	}

	void SetParent(Transform* parent)
	{
		//ToDo - Implement relative offset

		if (parent == Parent)
		{
			return;
		}

		if (Parent != nullptr)
		{
			Parent->ChildRemoved(this);
		}

		if (parent != nullptr)
		{
			parent->ChildAdded(this);
		}

		worldMatrixDirty = true;
		Parent = parent;
	}

	glm::vec3 GetLocalPosition()
	{
		return Position;
	}

	void SetLocalPosition(glm::vec3 pos)
	{
		worldMatrixDirty = true;
		localMatrixDirty = true;
		Position = pos;
	}

	void Translate(glm::vec3 pos)
	{
		Position += pos;
	}

	glm::vec3 GetLocalRotation()
	{
		return Rotation;
	}

	void SetLocalRotation(glm::vec3 rot)
	{
		worldMatrixDirty = true;
		localMatrixDirty = true;
		Rotation = rot;
	}

	glm::vec3 GetLocalScale()
	{
		return Scale;
	}

	void SetLocalScale(glm::vec3 scale)
	{
		localMatrixDirty = true;
		Scale = scale;
	}

	void UpdateIMGUI();

	glm::mat4 WorldMatrix();
	glm::mat4 LocalMatrix();

private:
	bool worldMatrixDirty = true;
	bool localMatrixDirty = true;

	glm::mat4 CachedWorldMatrix;
	glm::mat4 CachedLocalMatrix;

	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;

	Transform* Parent;
	std::vector<Transform*> Children;

	void RecurseImGuiScene();
	void ChildAdded(Transform* t);
	void ChildRemoved(Transform * t);
	void MakeWorldMatrixDirty();
	Transform* GetRoot();

	// Inherited via Component
	virtual void Start() override;
	virtual void Update() override;

};