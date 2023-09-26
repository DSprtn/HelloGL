#include "World.h"

World::~World()
{
	ClearAllEntities();
}

void World::Update()
{
	CleanupDeletedEntities();
	AddInstantiatedEntities();

	for (int i = 0; i < Entities.Count; i++) {
		Entities[i].Update();
	}
}

void World::AddInstantiatedEntities()
{
	for (int i = 0; i < Entities.Count; i++) {
		if (!Entities[i].Instantiated)
		{
			Entities[i].Instantiated = true;
			Entities[i].Start();
		}

	}
}

void World::CleanupDeletedEntities()
{
	for (int i = 0; i < Entities.Count; i++) {
		if (Entities[i].MarkedForDeletion) {\
			Entities.EraseAt(i);
			i--;
		}
	}
}

void World::LateUpdate()
{
	for (int i = 0; i < Entities.Count; i++) {
		if (Entities[i].Instantiated)
		{
			Entities[i].LateUpdate();
		}

	}
}

void World::OnRender()
{
	for (int i = 0; i < Entities.Count; i++) {
		if (Entities[i].Instantiated)
		{
			Entities[i].OnRender();
		}
	}
}

void World::ClearAllEntities()
{
	Entities.Clear();
}
