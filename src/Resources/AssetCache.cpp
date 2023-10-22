#include <AssetCache.h>

std::unordered_map<std::string, Model*> AssetCache::ModelCache = std::unordered_map<std::string, Model*>();
std::unordered_map<std::string, Texture*> AssetCache::TextureCache = std::unordered_map<std::string, Texture*>();

Model* AssetCache::LoadModel(std::string path)
{
	if (ModelCache.contains(path))
	{
		return ModelCache[path];
	}

	ModelCache.emplace(path, new Model(path));
	return ModelCache[path];
}

void AssetCache::Init()
{

}

Texture* AssetCache::LoadTexture(std::string path, bool sRGB)
{
	if (TextureCache.contains(path))
	{
		return TextureCache[path];
	}

	TextureCache.emplace(path, new Texture(path, sRGB));
	return TextureCache[path];
}
