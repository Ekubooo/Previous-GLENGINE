#include "LightSpot.h"

LightSpot::LightSpot(glm::vec3 position, glm::vec3 angle, glm::vec3 color)
: _position(position), _angles(angle), _color(color),_switchFlag(true)
{
    _FlagColor = _color;
    updateDirection();
}

void LightSpot::updateDirection()
{
    _directoion = glm::vec3(0,0,-1.0f);  
    _directoion = glm::rotateZ(_directoion,_angles.z);
    _directoion = glm::rotateX(_directoion,_angles.x);
    _directoion = glm::rotateY(_directoion,_angles.y);
    _directoion = -1.0f * _directoion;      // dir2Light, not light2Dir
}

