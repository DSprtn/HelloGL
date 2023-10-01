#include "World.h"

World::~World()
{
	ClearAllEntities();
}

void World::Update()
{
	CleanupDeletedEntities();
	AddInstantiatedEntities();


	for (int i = 0; i < Entities.size(); i++) {
		Entities[i]->Update();
	}
}

void World::AddInstantiatedEntities()
{
	for (size_t i = 0; i < InstantiatedEntities.size(); i++) {
		Entities.push_back(InstantiatedEntities[i]);
		InstantiatedEntities[i]->Start();
	}
	InstantiatedEntities.clear();
}

void World::CleanupDeletedEntities()
{
	Entities.erase(std::remove_if(std::begin(Entities), std::end(Entities), [](Entity* e) { return (e->MarkedForDeletion); }), Entities.end());
}

void World::LateUpdate()
{
	for (size_t i = 0; i < Entities.size(); i++) 
	{
		Entities[i]->LateUpdate();
	}
}

void World::OnRender()
{
	for (int i = 0; i < Entities.size(); i++) 
	{
			Entities[i]->OnRender();
	}
}

void World::ClearAllEntities()
{
	for (size_t i = 0; i < Entities.size(); i++) {
		delete Entities[i];
	}
	Entities.clear();
}
