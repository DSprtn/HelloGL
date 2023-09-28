#include <Component.h>
#include <Model.h>

class Renderer : Component
{
public:
	Renderer(Entity* owner, std::string modelPath, Shader* shader) : model{ modelPath }, Component(owner)
	{
		Shader = shader;
	}

	void Draw();

private:
	Model model;
	Shader* Shader;

	// Inherited via Component
	virtual void Start() override;

};