#pragma once
#include "Entity.h"

class Entity;

class Component
{
public:
	Component(Entity* owner)
	{
		m_Owner = owner;
	}
	virtual ~Component() {};


	virtual void Start() = 0;
	virtual void Update() {};
	virtual void LateUpdate() {};
	virtual void OnRender() {};
	virtual void OnCollide(Entity* B) {};

	Entity* m_Owner;

private:

	Component(const Component& that);
	Component& operator=(const Component& that);

};

