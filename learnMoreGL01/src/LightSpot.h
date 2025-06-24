#ifndef LIGHTSPOT_H
#define LIGHTSPOT_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

class LightSpot
{
public:
    glm::vec3 _position;
    glm::vec3 _color;
    glm::vec3 _angles;
    glm::vec3 _directoion;
    float _cosPhiInner = 1.0f;
    float _cosPhiOuter = 0.99f;

    glm::vec3 _FlagColor;
    bool _switchFlag;
public:
    LightSpot(glm::vec3 position,glm::vec3 angle,glm::vec3 color = glm::vec3(1.0f,1.0f,1.0f));
    ~LightSpot() = default;
    void updateDirection();
};

#endif