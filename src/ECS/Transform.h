#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <vector>

class Transform
{
public:

	Transform()
	{
		Position = glm::vec3(0.0f);
		Rotation = glm::vec3(0.0f);
		CachedWorldMatrix = glm::mat4(1.0f);
		Parent = nullptr;

	}

	glm::vec3 Position;
	glm::vec3 Rotation;

	glm::mat4 WorldMatrix();
	glm::mat4 LocalMatrix();

	Transform* Parent;
	std::vector<Transform*> Children;

private:
	bool matrixDirty = true;

	glm::mat3 CachedWorldMatrix;


};