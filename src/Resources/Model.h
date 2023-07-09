#pragma once

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <vector>
#include <Shader.h>
#include <filesystem>
#include <Mesh.h>
#include <unordered_map>

class Model
{
public:

	std::vector<Mesh> Meshes;

	std::unordered_map<std::string, Texture> CachedTextures;

	Model(const std::filesystem::path& path);

	void Draw(Shader& shader);

private:
	
	std::filesystem::path directory;

	void LoadModel(const std::filesystem::path& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
		std::string typeName);

};