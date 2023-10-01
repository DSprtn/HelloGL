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

		worldMatrixDirty = true;
		Parent = parent;
	}

	glm::vec3 GetPosition()
	{
		return Position;
	}

	void SetPosition(glm::vec3 pos)
	{
		localMatrixDirty = true;
		Position = pos;
	}

	glm::vec3 GetRotation()
	{
		return Position;
	}

	void SetRotation(glm::vec3 rot)
	{
		localMatrixDirty = true;
		Rotation = rot;
	}

	glm::vec3 GetScale()
	{
		return Scale;
	}

	void SetScale(glm::vec3 rot)
	{
		localMatrixDirty = true;
		Scale = rot;
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


	// Inherited via Component
	virtual void Start() override;

};