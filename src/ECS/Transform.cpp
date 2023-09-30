#include "Transform.h"

glm::mat4 Transform::WorldMatrix()
{
	return glm::mat4(1.0f);
}

glm::mat4 Transform::LocalMatrix()
{
	return glm::mat4(1.0f);
}
