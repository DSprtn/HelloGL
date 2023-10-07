#pragma once 

#include <glm/ext/vector_float2.hpp>


class Input
{
public:

	enum class InputContext
	{
		Menus,
		Ingame
	};

public:

	static Input* Instance;
	InputContext Context;

	Input();
	~Input();
	

	void Init();
	void Update();

	glm::vec2 ScrollInput;
	int GetKeyPressed(int key);
	int GetKeyPressed(int key, InputContext filter);
	void ToggleContext();

private:
	Input(Input& other) = delete;
	Input(const Input&& other) = delete;

	Input& operator=(const Input& other) = delete;
	Input& operator=(Input&& other) = delete;

};