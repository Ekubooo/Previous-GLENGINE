#include <iostream>

#include "Camera.h"

Camera::Camera
(glm::vec3 position, glm::vec3 target, glm::vec3 worldup)
:_Position(position), _Worldup(worldup)
{
    _Forward = glm::normalize(target - position);
    _Right = glm::normalize(glm::cross(_Forward, _Worldup));
    _Up = glm::normalize(glm::cross(_Forward, _Right));

}
Camera::Camera                  // 欧拉角版本
(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup)
:_Position(position), _Worldup(worldup), _Pitch(pitch), _Yaw(yaw)
{
    _Forward.x = glm::cos(_Pitch) * glm::sin(_Yaw);
    _Forward.y = glm::sin(_Pitch);
    _Forward.z = glm::cos(_Pitch) * glm::cos(_Yaw);
    _Right = glm::normalize(glm::cross(_Forward, _Worldup));
    _Up = glm::normalize(glm::cross(_Forward, _Right));
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
    _Up = glm::normalize(glm::cross(_Forward, _Right));
}

void Camera::ProcessMouseMovement(float deltaX, float deltaY)
{
    _Yaw -= deltaX * _SenseRow;
    _Pitch -= deltaY * _SenseRow;
    UpdateCameraVectors();
}
void Camera::updateCameraPos()
{
    //_Position += glm::vec3(0,0,-_SpeedZ * 10 * _SenseRow);
    _Position += _Forward * _SpeedZ * _SenseRow * 20.0f;
    _Position += _Right * _SpeedX * _SenseRow * 20.0f;
    _Position += _Up * _SpeedY * _SenseRow * 20.0f;
}




