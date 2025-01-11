#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
class Material
{
public:
    Shader* _myShader;
    glm::vec3 _ambient;
    unsigned int _specular;
    unsigned int _diffuse;
    float _shininess;
public:
    Material(Shader* myShader, unsigned int diffuse, 
        unsigned int specular, glm::vec3 ambient ,float shininess);
    ~Material();
private:

};

#endif