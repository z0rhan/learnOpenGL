#include "Model.hh"
#include "error.hh"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <stb_image/stb_image.h>
#include <iostream>

Model::Model(const char* filePath)
{
    loadModel(filePath);
}

Model::~Model()
{
}

//------------------------------------------------------------------------------
// Public

void Model::draw(Shader& shader)
{
    for (unsigned int i = 0; i < m_meshes.size(); i++)
        m_meshes.at(i).draw(shader);
}

//------------------------------------------------------------------------------
// Private

void Model::loadModel(std::string path)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags && AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "ERROR::ASSIMP " << importer.GetErrorString() << "\n";
        return;
    }
    m_directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // Vertex
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vertexVector;

        // Positions
        vertexVector.x = mesh->mVertices[i].x;
        vertexVector.y = mesh->mVertices[i].y;
        vertexVector.z = mesh->mVertices[i].z;
        vertex.Position_ = vertexVector;

        // Normals
        vertexVector.x = mesh->mNormals[i].x;
        vertexVector.y = mesh->mNormals[i].y;
        vertexVector.z = mesh->mNormals[i].z;
        vertex.Normal_ = vertexVector;

        // Textures
        if (mesh->mTextureCoords[0])
        {
            glm::vec2 textureVector; 
            textureVector.x = mesh->mTextureCoords[0][i].x;
            textureVector.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords_ = textureVector;
        }
        else
            vertex.TexCoords_ = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    // Indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Material
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffuseMaps = loadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTexture(aiMaterial* material, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < material->GetTextureCount(type); i ++)
    {
        aiString str;
        bool skip = false;

        material->GetTexture(type, i, &str);
        for (unsigned int j = 0; j < m_loadedTextures.size(); j++)
        {
            if (std::strcmp(m_loadedTextures[j].path_.data(), str.C_Str()) == 0)
            {
                textures.push_back(m_loadedTextures[j]);
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            Texture texture;
            texture.id_ = loadTexture(str.C_Str(), m_directory);
            texture.type_ = typeName;
            texture.path_ = str.C_Str();
            textures.push_back(texture);
        }
    }

    return textures;
}

unsigned int Model::loadTexture(const char* file, const std::string& directory, bool gamma)
{
    std::string filePath = directory + '/' + std::string(file);

    unsigned int textureId;
    int widht, height, nrComponents;

    glCall(glGenTextures(1, &textureId));
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(filePath.c_str(), &widht, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        else
            std::cerr << "Error in nrComponents \n";

        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, format, widht, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }
    else
    {
        std::cerr << "Error loading texuture: " << filePath << "\n";
        std::cerr << stbi_failure_reason() << "\n";
        stbi_image_free(data);
    }

    return textureId;
}
