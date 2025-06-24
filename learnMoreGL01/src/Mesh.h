#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"

struct Vertex {
    glm::vec3 _Position;
    glm::vec3 _Normal;
    glm::vec2 _TexCoords; 
};
struct Texture {
    unsigned int id;
    std::string type;
    aiString path;
    //std::string path;
};

class Mesh
{
public:
    std::vector<Vertex> vertices;       // VBO data
    std::vector<unsigned int> indices;  // EBO data
    std::vector<Texture> textures;           
public:
    Mesh(float vert[]);
    Mesh(std::vector<Vertex> vertices,
         std::vector<unsigned int> indices,
         std::vector<Texture> textures);
    void Draw(Shader* myShader);
private:
    unsigned int VAO = 1, VBO = 1, EBO = 1;
private:
    void setupMesh();
};

#endif