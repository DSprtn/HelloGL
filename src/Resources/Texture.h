#pragma once 

#include <string>


class Texture {
public:

	Texture(const std::string& path, bool sRGB)
	{
		LoadTexture(path, sRGB);
	}

	void LoadTexture(const std::string& path, const bool sRGB);

	unsigned int id;
	std::string type;
	std::string path;
	std::string drawName;
};