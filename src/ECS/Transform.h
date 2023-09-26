class Transform
{
	public Transform()
	{
		Position = glm::vec3(0.0f);
		Rotation = glm::vec3(0.0f);
		CachedWorldMatrix = glm::mat4(1.0f);
	}

public:
	bool matrixDirty = true;


	glm::mat3 CachedWorldMatrix;
	glm::vec3 Position;
	glm::vec3 Rotation;

	glm::mat4 WorldMatrix();
	glm::mat4 LocalMatrix();

	Transform* Parent;
	std::vector<Transform*> Children;
}