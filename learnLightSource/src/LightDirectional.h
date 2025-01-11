#ifndef LIGHT_H
#define LIGHT_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

class LightDirectional
{
public:
    glm::vec3 _position;
    glm::vec3 _color;
    glm::vec3 _angles;
    glm::vec3 _directoion = glm::vec3(0,0,1.0f);
public:
    LightDirectional(glm::vec3 position,glm::vec3 angle,glm::vec3 color = glm::vec3(1.0f,1.0f,1.0f));
    ~LightDirectional() = default;
    void updateDirection();
private:

};

#endif