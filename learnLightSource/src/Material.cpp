#include "Material.h"

Material::Material(Shader* myShader, unsigned int diffuse, 
    unsigned int specular, glm::vec3 ambient ,float shininess)
:_myShader(myShader), 
_ambient(ambient), 
_specular(specular), 
_shininess(shininess),
_diffuse(diffuse)
{

}

Material::~Material()
{
    delete _myShader;
}
