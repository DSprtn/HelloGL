#pragma once 


#include <Transform.h>
#include <Component.h>
#include <Model.h>
#include <Shader.h>

#include <AssetCache.h>

class MeshRenderer : public Component
{
public:
	MeshRenderer(Entity* owner, std::string modelPath, Shader* shader);
	~MeshRenderer();

	void Draw();
	bool Visible = true;
	bool CastShadow = true;


private:
	Model* model;
	Shader* shader;


	// Inherited via Component
	virtual void Start() override;

	virtual void Update() override;
	virtual void OnRender() override;
};