#pragma once

#include "Shader.hh"
#include <glm/glm.hpp>
#include <string>
#include <vector>

typedef struct
{
    glm::vec3 Position_;
    glm::vec3 Normal_;
    glm::vec2 TexCoords_;

} Vertex;

typedef struct
{
    unsigned int id_;
    std::string type_;
    std::string path_;

} Texture;

class Mesh
{
public:
    Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures);
    ~Mesh();

    void draw(Shader& shader, unsigned int count = 0);

private:
    void setupMesh();

private:
    // Mesh data
    std::vector<Vertex> m_vertices;
    std::vector<Texture> m_textures;
    std::vector<unsigned int> m_indices;

    // Render data
    unsigned int m_VAO, m_VBO, m_EBO;

};
