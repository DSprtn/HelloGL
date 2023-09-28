#pragma once

#include <Entity.h>

class World
{
public:
	World() = default;
	~World();

	std::vector<std::unique_ptr<Entity>> Entities;

	void Update();
	void LateUpdate();
	void OnRender();
	void ClearAllEntities();

	template <typename T, class... Args>
	T* CreateEntity(Args&&... args) {
		return & (Entities.emplace_back(std::forward<Args>(args)...));
	}

private:
	void AddInstantiatedEntities();
	void CleanupDeletedEntities();


	World(const World& that);
	World& operator=(const World& that);
};

