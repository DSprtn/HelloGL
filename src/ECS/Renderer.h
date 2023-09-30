#include <Component.h>
#include <Model.h>
#include <Shader.h>

class Renderer : public Component
{
public:
	Renderer(Entity* owner, std::string modelPath) : model{ modelPath }, Component(owner)
	{
	}

	void Draw(Shader& shader);

private:
	Model model;

	// Inherited via Component
	virtual void Start() override;

};