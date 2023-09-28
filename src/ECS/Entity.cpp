#include "Entity.h"
#include <iostream>

Entity::Entity(std::string name)
{
	Name = name;
	MarkedForDeletion = false;
	Instantiated = false;
}

Entity::~Entity()
{
	for (int i = 0; i < Components.size(); i++) {
		delete Components[i];
	}
	Components.clear();
}

void Entity::Start()
{
	for (int i = 0; i < Components.size(); i++) {
		Components[i]->Start();
	}
}

void Entity::Update()
{
	for (int i = 0; i < Components.size(); i++) {
		Components[i]->Update();
	}
}

void Entity::LateUpdate()
{
	for (int i = 0; i < Components.size(); i++) {
		Components[i]->LateUpdate();
	}
}

void Entity::OnRender()
{
	for (int i = 0; i < Components.size(); i++) {
		Components[i]->OnRender();
	}
}

void Entity::Delete() {
	MarkedForDeletion = true;
}
