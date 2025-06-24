#include "Camera.h"

Camera::Camera
(glm::vec3 position, glm::vec3 target, glm::vec3 worldup)
:_Position(position), _Worldup(worldup)
{
    _Forward = glm::normalize(target - position);
    _Right = glm::normalize(glm::cross(_Forward, _Worldup));
    _Up = glm::normalize(glm::cross( _Right,_Forward));

}
Camera::Camera                  // Euler angle overloading
(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup)
:_Position(position), _Worldup(worldup), _Pitch(pitch), _Yaw(yaw)
{
    _Forward.x = glm::cos(_Pitch) * glm::sin(_Yaw);
    _Forward.y = glm::sin(_Pitch);
    _Forward.z = glm::cos(_Pitch) * glm::cos(_Yaw);
    _Right = glm::normalize(glm::cross(_Forward, _Worldup));
    _Up = glm::normalize(glm::cross( _Right,_Forward));
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(_Position, _Position + _Forward, _Worldup);
}

void Camera::UpdateCameraVectors()
{
    _Forward.x = glm::cos(_Pitch) * glm::sin(_Yaw);
    _Forward.y = glm::sin(_Pitch);
    _Forward.z = glm::cos(_Pitch) * glm::cos(_Yaw);
    _Right = glm::normalize(glm::cross(_Forward, _Worldup));
    _Up = glm::normalize(glm::cross( _Right,_Forward));
}

void Camera::ProcessMouseMovement(float deltaX, float deltaY)
{
    _Yaw -= deltaX * _SenseRow;
    _Pitch -= deltaY * _SenseRow;
    UpdateCameraVectors();
}

void Camera::updateCameraPos()
{
    _Position += _Forward * _SpeedZ * _SenseRow * 20.0f;
    _Position += _Right * _SpeedX * _SenseRow * 20.0f;
    _Position += _Up * _SpeedY * _SenseRow * 20.0f;
}

// try
void Camera::updateSpotLightPos(LightSpot *MySpLight)
{
    MySpLight->_position += _Forward * _SpeedZ * _SenseRow * 20.0f;
    MySpLight->_position += _Right * _SpeedX * _SenseRow * 20.0f;
    MySpLight->_position += _Up * _SpeedY * _SenseRow * 20.0f;
    /* 
    std::cout<<"Spot dir start :"<<"///////"
        <<MySpLight->_directoion.x<<"///////"
        <<MySpLight->_directoion.y<<"///////"
        <<MySpLight->_directoion.z<<"///////"
        <<std::endl;
     */
    MySpLight->_directoion.x = -1.0f * glm::cos(_Pitch) * glm::sin(_Yaw);
    MySpLight->_directoion.y = -1.0f * glm::sin(_Pitch);
    MySpLight->_directoion.z = -1.0f * glm::cos(_Pitch) * glm::cos(_Yaw);
}

