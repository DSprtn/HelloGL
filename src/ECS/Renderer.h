#pragma once 

#include <Transform.h>
#include <Component.h>
#include <Model.h>
#include <Shader.h>
#include <AssetCache.h>

class Renderer : public Component
{
public:
	Renderer(Entity* owner, std::string modelPath, Shader* shader);

	void Draw();
	bool Visible = true;
	bool CastShadow = true;


private:
	Model* model;
	Shader* shader;


	// Inherited via Component
	virtual void Start() override;

	virtual void Update() override;
};