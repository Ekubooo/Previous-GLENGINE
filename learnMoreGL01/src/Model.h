#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "stb_image.h"
#include "Mesh.h"
#include "Shader.h"


class Model
{
public:
    std::vector<Mesh> _meshes;
    std::vector<Texture> textures_loaded;
    std::string _directory;
    bool _gammaCorrection;
public:
    //Model(const std::string& path);
    Model(const std::string &path, bool gamma = false);
    ~Model() = default;
    void Draw(Shader* shader);
private:
    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    void processVertics
        (aiMesh* mesh, std::vector<Vertex>& tempVert); 
    void processIndices
        (aiMesh *mesh, std::vector<unsigned int>& tempIndices);
    void processTexture
        (aiMesh *mesh, const aiScene *scene, std::vector<Texture> &tempTexture);
    std::vector<Texture> loadMaterialTextures
        (aiMaterial *mat, aiTextureType type, std::string typeName);
    unsigned int TextureFromFile
        (const char *path, const std::string &directory, bool gamma);
};

#endif