#pragma once

#include <Entity.h>

class World
{
public:
	World() = default;
	~World();

	std::vector<Entity*> Entities;
	std::vector<Entity*> InstantiatedEntities;

	void Update();
	void LateUpdate();
	void OnRender();
	void ClearAllEntities();

	template <typename T, class... Args>
	T* CreateEntity(Args&&... args) {
		T* entity(new T(std::forward<Args>(args)...));
		InstantiatedEntities.push_back(entity);
		return entity;
	}

private:
	void AddInstantiatedEntities();
	void CleanupDeletedEntities();


	World(const World& that);
	World& operator=(const World& that);
};

