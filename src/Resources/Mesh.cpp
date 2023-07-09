#include "Mesh.h"
#include <vector>


void Mesh::SetupMesh()
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int),
    &Indices[0], GL_STATIC_DRAW);

  // vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  // vertex normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
  // vertex texture coords
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

  glBindVertexArray(0);
}

void Mesh::Draw(Shader& shader)
{
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  unsigned int emissiveNr = 1;
  for (unsigned int i = 0; i < Textures.size(); i++)
  {
    glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
    // retrieve texture number (the N in diffuse_textureN)
    std::string number;
    std::string name = Textures[i].type;
    if (name == "diffuse")
      number = std::to_string(diffuseNr++);
    else if (name == "specular")
      number = std::to_string(specularNr++);
    else if (name == "emissive")
      number = std::to_string(emissiveNr++);

    shader.setUniform1i(i,("material." + name).c_str());
    glBindTexture(GL_TEXTURE_2D, Textures[i].id);
  }
  glActiveTexture(GL_TEXTURE0);

  // draw mesh
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
