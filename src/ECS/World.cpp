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
	for (int i = 0; i < Entities.size(); i++) {
		if (!Entities[i]->Instantiated)
		{
			Entities[i]->Instantiated = true;
			Entities[i]->Start();
		}

	}
}

void World::CleanupDeletedEntities()
{
	Entities.erase(std::remove_if(std::begin(Entities), std::end(Entities), [](std::unique_ptr<Entity>& e) { return (e->MarkedForDeletion); }), Entities.end());
}

void World::LateUpdate()
{
	for (int i = 0; i < Entities.size(); i++) {
		if (Entities[i]->Instantiated)
		{
			Entities[i]->LateUpdate();
		}

	}
}

void World::OnRender()
{
	for (int i = 0; i < Entities.size(); i++) {
		if (Entities[i]->Instantiated)
		{
			Entities[i]->OnRender();
		}
	}
}

void World::ClearAllEntities()
{
	Entities.clear();
}
