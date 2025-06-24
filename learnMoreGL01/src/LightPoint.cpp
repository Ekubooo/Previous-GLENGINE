#include "LightPoint.h"

LightPoint::LightPoint(glm::vec3 position, glm::vec3 angle, glm::vec3 color)
: _position(position), _angles(angle), _color(color),
 _constant(1.0f), _linear(0.15f), _quadratic(0.032f)
{

}

