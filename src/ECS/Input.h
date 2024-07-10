#pragma once 

#include <glm/ext/vector_float2.hpp>
#include <array>


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

	bool GetKeyDown(int key);
	bool GetKeyDown(int key, InputContext filter);

	void ToggleContext();

	std::array<bool, 500> keyPresses;

private:
	Input(Input& other) = delete;
	Input(const Input&& other) = delete;

	Input& operator=(const Input& other) = delete;
	Input& operator=(Input&& other) = delete;

};