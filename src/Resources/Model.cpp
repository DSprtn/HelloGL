#include "Model.h"
#include <stb_image.h>


namespace
{

	unsigned int LoadTexture(char const* path, bool sRGB = false)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format = GL_RED;

			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			GLenum internalFormat = format;

			if (sRGB)
			{
				if (nrComponents == 3)
				{
					internalFormat = GL_SRGB;
				}
				else if (nrComponents == 4)
				{
					internalFormat = GL_SRGB_ALPHA;
				}
			}

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}
}


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
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_FlipUVs |
		aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.parent_path();
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
		
		float u = 0.0f, v = 0.0f;
		if (mesh->HasTextureCoords(0))
		{
			const aiVector3D& UV = mesh->mTextureCoords[0][i];
			u = UV.x;
			v = UV.y;
		}

		m.Vertices.emplace_back(Vertex(pos.x, pos.y, pos.z, normal.x, normal.y, normal.z, u, v));
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
		if (type == aiTextureType_NORMALS)
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
			Texture texture;
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
