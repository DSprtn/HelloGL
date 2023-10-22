#pragma once

#include <vector>
#include <Shader.h>
#include <filesystem>
#include <Texture.h>


struct Vertex
{

	Vertex(float posX, float posY, float posZ, float normX, float normY, float normZ, float texU, float texV, float tangX, float tangY, float tangZ)
	{
		Position = glm::vec3(posX, posY, posZ);
		Normal = glm::vec3(normX, normY, normZ);
		TexCoords = glm::vec2(texU, texV);
		Tangent = glm::vec3(tangX, tangY, tangZ);
	}

	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
};

class Mesh
{
public:

	std::vector<Vertex> Vertices;
	std::vector<uint32_t> Indices;
	std::vector<Texture> Textures;

	unsigned int VAO = 0, VBO = 0, EBO = 0;

	Mesh() {};

	Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures)
	{
		Vertices = vertices;
		Indices = indices;
		Textures = textures;
	};

	Mesh& operator=(Mesh const& copy) = delete;
	Mesh(Mesh const& copy) = delete;

	
	Mesh(Mesh&& m) noexcept
	{
		Swap(m);
	}

	void SetupMesh();

	void Draw(Shader& shader);

private:
	void Swap(Mesh& other)
	{
		std::swap(Vertices, other.Vertices);
		std::swap(Textures, other.Textures);
		std::swap(Indices, other.Indices);
		std::swap(VAO, other.VAO);
		std::swap(VBO, other.VBO);
		std::swap(EBO, other.EBO);
	}

};

