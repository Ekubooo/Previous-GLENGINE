#ifndef LIGHTPOINT_H
#define LIGHTPOINT_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
class LightPoint
{
public:
    glm::vec3 _position;
    glm::vec3 _color;
    glm::vec3 _angles;
    glm::vec3 _directoion = glm::vec3(0,0,1.0f);
    float _constant;
    float _linear;
    float _quadratic;
public:
    LightPoint (glm::vec3 position,glm::vec3 angle,glm::vec3 color = glm::vec3(1.0f,1.0f,1.0f));
    ~LightPoint () = default;
};

#endif