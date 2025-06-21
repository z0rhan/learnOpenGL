#pragma once

#include "Mesh.hh"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <string>

class Model
{
public:
    Model(const char* filePath);
    ~Model();

    void draw(Shader& shader);

private:
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTexture(aiMaterial* material, aiTextureType type, std::string typeName);
    unsigned int loadTexture(const char* file, const std::string& directory, bool gamma = false);

private:
    std::vector<Mesh>       m_meshes;
    std::string             m_directory;
    std::vector<Texture>    m_loadedTextures;
};
