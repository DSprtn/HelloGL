#include <glad/glad.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include <iostream>

#include <math.h>
#include "HelloGL.h"
#include "Shaders/Shader.h"
#include <algorithm>
#include <numeric>
#include <array>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#include <Model.h>
#include <Entity.h>
#include <World.h>
#include <MeshRenderer.h>
#include <Renderer.h>
#include <Input.h>
#include <Engine.h>
#include <Camera.h>
#include <Timer.h>

int main(int argc, char* argv[])
{
	Engine engine;
	engine.Init();

	Shader defaultProgram = Shader("Shaders/Technicolor.vert", "Shaders/Technicolor.frag");
	Shader lightProgram = Shader("Shaders/Light.vert", "Shaders/Light.frag");
	Shader uberProgram = Shader("Shaders/UberShader.vert", "Shaders/UberShader.frag");

#pragma region CreateScene

	auto createEntityWithModel = [&](std::string name, std::string model, Shader* shader)
	{
		auto e = engine.CurrentWorld->CreateEntity<Entity>(name);
		e->AddComponent<MeshRenderer>(model, shader);
		return e;
	};

	auto createEntity = [&](std::string name)
	{
		auto e = engine.CurrentWorld->CreateEntity<Entity>(name);
		return e;
	};

	auto createLight = [&](std::string name, Light::LightType type)
	{
		auto e = engine.CurrentWorld->CreateEntity<Entity>(name);
		return e->AddComponent<Light>(type);
	};

	auto camEntity = createEntity("MainCam");
	auto cam = camEntity->AddComponent<Camera>();
	camEntity->Transform->SetLocalPosition(glm::vec3(-11.0f, 1.6f, 0.46f));
	camEntity->Transform->SetLocalRotation(glm::vec3(0.0f, -90.0f, 0.0f));

	auto testLight = createLight("Light", Light::PointLight);
	testLight->Color = glm::vec3(.3f, .3f, .7f);
	

	auto sponza = createEntityWithModel("Sponza", "assets/model/sponza/sponza.obj", &uberProgram);
	sponza->Transform->SetLocalScale(glm::vec3(0.01f));

	Transform* parent = nullptr;
	for (int i = 0; i < 9; i++)
	{
		auto cubeChild = createEntityWithModel("Cube" + std::to_string(i), "assets/model/cube/cube.obj", &uberProgram);
		cubeChild->Transform->SetParent(parent);
		cubeChild->Transform->SetLocalPosition(glm::vec3(0, 1.0f, 0));
		cubeChild->Transform->SetLocalScale(glm::vec3(.8f));
		parent = cubeChild->Transform;
	}

	auto lightParent = createEntityWithModel("LightParent", "assets/model/icosphere/icosphere.obj", &uberProgram);

	for (int i = 5; i < 31; i++)
	{
		auto light = createLight("Light " + std::to_string(i), Light::PointLight);
		light->Range = 1.75f;
		Transform& t = *light->m_Owner->Transform;
		t.SetParent(lightParent->Transform);

		float x, y, z;
		x = sinf(i / 1.5f) * (i / 3.0f);
		y = .25f;
		z = cosf(i / 1.5f) * (i / 3.0f);

		t.SetLocalPosition(glm::vec3(x,y,z));
		light->Color = (glm::vec3(x + 1, y, z) + glm::vec3(1.0f)) / 2.0f;
	}
	
#pragma endregion


	Core::Time::Init();
	while (!glfwWindowShouldClose(engine.Window))
	{
		Core::Time::ComputeDeltas();

		lightParent->Transform->Rotate(glm::vec3(0.0f, 10.0f * Core::Time::DeltaTime, 0.0f));


		engine.Update();
		engine.LateUpdate();
		engine.OnRender();
	}

	return 0;
}




