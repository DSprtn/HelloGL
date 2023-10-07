#pragma once 

#include <Shader.h>
#include <vector>
#include <Light.h>
#include <MeshRenderer.h>
#include <Camera.h>


class Renderer
{
public:
	static Renderer* Instance;
	Camera* MainCamera;
	
	Renderer();


	void Init();
	void Render();
	void Update();

	void RegisterShader(Shader* s);
	void UnregisterShader(Shader* s);

	void RegisterMeshRenderer(MeshRenderer* m);
	void UnregisterMeshRenderer(MeshRenderer* m);

	void RegisterLight(Light* light);
	void UnregisterLight(Light* light);


	void SetCamera(Camera* cam);

private:

	std::vector<Light*> Lights;
	std::vector<MeshRenderer*> Meshes;
	std::vector<Shader*> Shaders;
};