#include "Mesh.h"

Mesh::Mesh(float vert[])
{   
    this->vertices.resize(36);
    memcpy(&(this->vertices[0]), vert, 36 * 8 * sizeof(float));
    setupMesh();        // init mesh into OpenGL's State machine
}

Mesh::Mesh(std::vector<Vertex> vertices,
           std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    this->vertices = vertices;      // deep copy
    this->indices = indices;        // deep copy
    this->textures = textures;      // deep copy
    setupMesh();                    // init mesh into OpenGL's State machine
}
void Mesh::Draw(Shader* myShader)
{
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        if (textures[i].type == "texture_diffuse")
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
            myShader->setUniform1Img("material.diffuse",Shader::DIFFUSE);
        }
        else if (textures[i].type == "texture_specular")
        {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
            myShader->setUniform1Img("material.specular",Shader::SPECULAR);
        }
    }
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, 36);    // cube test
    glBindVertexArray(0);                   // release: unbind 
    glActiveTexture(GL_TEXTURE0);           // release: unbind 
}

void Mesh::setupMesh()              // init mesh into OpenGL's State machine
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(),
         &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), 
        &indices[0], GL_STATIC_DRAW);
        
    // (void*)0 -> can replease by functor (LearnGL code) maybe try?
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(GL_FLOAT)));

    glBindVertexArray(0);       // finish the job, so release: unbind 
}
