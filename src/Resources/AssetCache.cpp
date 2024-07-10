#include <AssetCache.h>

std::unordered_map<std::string, Model*> AssetCache::ModelCache = std::unordered_map<std::string, Model*>();

Model* AssetCache::LoadModel(std::string path)
{
	if (ModelCache.contains(path))
	{
		return ModelCache[path];
	}

	ModelCache.emplace(path, new Model(path));
	return ModelCache[path];
}