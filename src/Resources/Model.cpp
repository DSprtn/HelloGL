#include "Model.h"
#include <stb_image.h>

Model::Model(const std::filesystem::path& path)
{
	LoadModel(path);
}

void Model::Draw(Shader& shader)
{
	for (auto& mesh : Meshes)
	{
		mesh.Draw(shader);
	}
}

void Model::LoadModel(const std::filesystem::path& path)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path.string(),
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_GenNormals |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.parent_path();



	/*
	* Limit amount of materials
	* If not bindless, we'll need a material per texture combination...?
	*		--- We can maybe keep track of purely the texture changes?
	* 
	* Allow using different materials
	*		--- Non-textured
	*		--- Emissive only
	*  
	* MODEL
	*		MATERIAL*
	*		1..*
	*		MESH*
	* 
	* Different textures per material
	*		Bindless approach would be better...?
	* 
	*		
	* 
	* 
	* 
	* 
	* 
	* 
	* */


	for (uint32_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* m = scene->mMaterials[i];
		
		for (uint32_t j = 0; j < m->mNumProperties; j++)
		{
			auto prop = m->mProperties[j];
			
		}
	}

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	

	for (uint32_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		Meshes.emplace_back(processMesh(mesh, scene));
	}

	for (uint32_t i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	Mesh m;
	m.Vertices.reserve(mesh->mNumVertices);
	m.Indices.reserve(mesh->mNumFaces * 3);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		
		// process vertex positions, normals and texture coordinates
		const aiVector3D& pos = mesh->mVertices[i];
		const aiVector3D& normal = mesh->mNormals[i];
		const aiVector3D& tangent = mesh->mTangents[i];
		
		float u = 0.0f, v = 0.0f;
		if (mesh->HasTextureCoords(0))
		{
			const aiVector3D& UV = mesh->mTextureCoords[0][i];
			u = UV.x;
			v = UV.y;
		}

		m.Vertices.emplace_back(Vertex(pos.x, pos.y, pos.z, normal.x, normal.y, normal.z, u, v, tangent.x, tangent.y, tangent.z));
	}
	
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const aiFace& face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			m.Indices.emplace_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "diffuse");
		m.Textures.insert(m.Textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> normalMaps = loadMaterialTextures(material,
			aiTextureType_DISPLACEMENT, "normal");
		m.Textures.insert(m.Textures.end(), normalMaps.begin(), normalMaps.end());

		normalMaps = loadMaterialTextures(material,
			aiTextureType_NORMALS, "normal");
		m.Textures.insert(m.Textures.end(), normalMaps.begin(), normalMaps.end());

		std::vector<Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "specular");
		m.Textures.insert(m.Textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector<Texture> emissiveMaps = loadMaterialTextures(material,
			aiTextureType_EMISSIVE, "emissive");
		m.Textures.insert(m.Textures.end(), emissiveMaps.begin(), emissiveMaps.end());

	}

	m.SetupMesh();
	return m;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		bool sRGB = true;
		if (type == aiTextureType_NORMALS || type == aiTextureType_DISPLACEMENT)
		{
			sRGB = false;
		}

		auto texPath = std::string((directory / str.C_Str()).string().c_str());

		if (CachedTextures.contains(texPath))
		{
			textures.push_back(CachedTextures[texPath]);
		}
		else
		{
			Texture texture(texPath.c_str(),sRGB);
			texture.id = LoadTexture(texPath.c_str(), sRGB);
			texture.type = typeName;
			texture.path = texPath;
			texture.drawName = "material." + typeName;
			textures.push_back(texture);

			CachedTextures[texPath] = texture;
		}
	}
	return textures;
}
