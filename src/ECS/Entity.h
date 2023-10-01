#pragma once

#include <string>
#include <typeinfo>
#include <iostream>
#include <unordered_set>
#include "Transform.h"
#include "Component.h"

class Component;
class Transform;

class Entity
{
public:

	Entity(std::string Name);
	virtual ~Entity();

	bool MarkedForDeletion;
	bool Instantiated;

	std::string Name;

	std::vector<Component*> Components;
	
	Transform* Transform;

	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();
	virtual void OnRender();
	virtual void Delete();

	template<typename T>
	T* GetComponent()
	{
		for (int i = 0; i < Components.size(); i++) {
			Component* c = Components[i];
			if (typeid(T).name() == typeid(*c).name()) {
				return static_cast<T*>(c);
			}
		}
		return nullptr;
	}

	template <typename T, class... Args>
	T* AddComponent(Args&&... args) {
		T* component(new T(this, std::forward<Args>(args)...));
		Components.emplace_back(component);
		return component;
	}


private:

	Entity(const Entity& that);
	Entity& operator=(const Entity& that);


};