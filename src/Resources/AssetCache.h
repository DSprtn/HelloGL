#pragma once 

#include <Model.h>
#include <string>

class AssetCache
{
public:

	static Texture WhiteTexture1x1;
	static Texture BlueTexture1x1;

	static void Init();

	static Model* LoadModel(std::string path);

	static Texture* LoadTexture(std::string path, bool sRGB);

private:

	static std::unordered_map<std::string, Model*> ModelCache;
	static std::unordered_map<std::string, Texture*> TextureCache;
};