
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glfw3.h>

class Camera
{
public:

	Camera(GLFWwindow* window);

	void LookAt(glm::vec3 position);

	void SetProjection(float FOV, float aspect, float near, float far);

	void Update(double delta);

	glm::mat4 ProjectionMatrix()
	{
		return m_projectionMatrix;
	}

	glm::mat4 Matrix()
	{
		return m_matrix;
	}

private:
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_matrix;
	glm::vec3 position;
	glm::quat rotation;
	float pitch = 0;
	float yaw = 0;
	GLFWwindow* window;

};