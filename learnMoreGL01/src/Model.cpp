#include "Model.h"

Model::Model(const std::string &path, bool gamma)
:_gammaCorrection(gamma)
{
    loadModel(path);
}

void Model::Draw(Shader *shader)
{
    for (unsigned int i = 0; i < _meshes.size(); i++)
    {
        _meshes[i].Draw(shader);
    }
}

void Model::loadModel(const std::string& path)
{
    Assimp::Importer importer;
    // Library design as const, return a const obj/pointer (?)
    const aiScene* scene = importer.ReadFile
        (path, aiProcess_Triangulate |aiProcess_FlipUVs|aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode )
    {
        std::cout<<"----assimp error" 
            <<importer.GetErrorString()<<std::endl;
            return;
    }
    _directory = path.substr(0, path.find_last_of('/'));
    //std::cout<<"--load Model success: "<<directory<<std::endl;
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    //std::cout<<node->mName.data<<std::endl;
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* cruMesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.push_back(ProcessMesh(cruMesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {   
        // Depth-First Traversal: process the obj-scene's and there child node 
        processNode(node->mChildren[i],scene);
    }
}

Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> tempVerics;
    std::vector<unsigned int> tempIndices;
    std::vector<Texture> tempTexture;
    
    // process Vertics
    processVertics(mesh, tempVerics);
    // process Indices
    processIndices(mesh,tempIndices);
    // process texture
    processTexture(mesh,scene,tempTexture);

    return Mesh(tempVerics,tempIndices,tempTexture);
    //return Mesh(tempVerics,tempIndices,{});
}

void Model::processVertics(aiMesh *mesh, std::vector<Vertex> &tempVert)
{
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        glm::vec3 tempVec;
        glm::vec2 tempUV;
        Vertex vertex;

        tempVec.x = mesh->mVertices[i].x;
        tempVec.y = mesh->mVertices[i].y;
        tempVec.z = mesh->mVertices[i].z;
        vertex._Position = tempVec;

        tempVec.x = mesh->mNormals[i].x;
        tempVec.y = mesh->mNormals[i].y;
        tempVec.z = mesh->mNormals[i].z;
        vertex._Normal = tempVec;

        if (mesh->mTextureCoords[0])
        {
            tempUV.x = mesh->mTextureCoords[0][i].x;
            tempUV.y = mesh->mTextureCoords[0][i].y;
            vertex._TexCoords = tempUV;
        }else{
            vertex._TexCoords = glm::vec2(0.0f,0.0f);
        }
        tempVert.push_back(vertex);
    }
}

void Model::processIndices(aiMesh *mesh, std::vector<unsigned int> &tempIndices)
{
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
        {
            tempIndices.push_back(mesh->mFaces[i].mIndices[j]);
        }
    }
}

void Model::processTexture
    (aiMesh *mesh, const aiScene *scene, std::vector<Texture> &tempTexture)
{   
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        
        // 1. diffuse maps
        std::vector<Texture> diffuseMaps 
            = loadMaterialTextures(material, aiTextureType_DIFFUSE,"texture_diffuse");
        tempTexture.insert(tempTexture.end(), diffuseMaps.begin(), diffuseMaps.end());

        // 2. specular maps
        std::vector<Texture> specularMaps 
            = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        tempTexture.insert(tempTexture.end(), specularMaps.begin(), specularMaps.end());

        // 3. normal maps
        std::vector<Texture> normalMaps 
            = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        tempTexture.insert(tempTexture.end(), normalMaps.begin(), normalMaps.end());
        
        // 4. height maps
        std::vector<Texture> heightMaps 
            = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        tempTexture.insert(tempTexture.end(), heightMaps.begin(), heightMaps.end());
    }
}

std::vector<Texture> Model::loadMaterialTextures
    (aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if(std::strcmp(textures_loaded[j].path.data, str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; 
                break;
            }
        }
        if(!skip)
        {
            Texture texture;
            texture.id = TextureFromFile
                (str.C_Str(), _directory, _gammaCorrection);
            texture.type = typeName;
            texture.path = str;
            textures.push_back(texture);
            textures_loaded.push_back(texture); 
        }
    }
    return textures;
}

unsigned int Model::TextureFromFile
    (const char *path, const std::string &directory, bool gamma)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
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