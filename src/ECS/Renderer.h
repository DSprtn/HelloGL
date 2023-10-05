#pragma once 

#include <vector>
#include <Light.h>
#include <MeshRenderer.h>
#include <Shader.h>


class Renderer
{
public:
	static Renderer* Instance;
	
	Renderer();


	void Init();
	void Render();

	void RegisterShader(Shader* s);
	void UnregisterShader(Shader* s);

	void RegisterMeshRenderer(MeshRenderer* m);
	void UnregisterMeshRenderer(MeshRenderer* m);

	void RegisterLight(Light* light);
	void UnregisterLight(Light* light);

private:
	std::vector<Light*> Lights;
	std::vector<MeshRenderer*> Meshes;
	std::vector<Shader*> Shaders;
};