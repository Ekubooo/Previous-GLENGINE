#ifndef CAMERAA_H
#define CAMERAA_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "LightSpot.h"

class Camera
{
public:
    glm::vec3 _Position;
    glm::vec3 _Forward;
    glm::vec3 _Right;
    glm::vec3 _Up;
    glm::vec3 _Worldup;
    float _Pitch, _Yaw;
    float _SenseRow = 0.001f;
    float _SpeedZ = 0.0f, _SpeedX = 0.0f,_SpeedY = 0.0f;
    enum Camera_Movement 
    {
        FORWARD,BACKWARD,
        LEFT,RIGHT
    };
public:
    Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup);
    Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup);
    ~Camera() = default;
    glm::mat4 getViewMatrix();
    void ProcessMouseMovement(float deltaX, float deltaY);
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void updateCameraPos();
    // try
    void updateSpotLightPos(LightSpot* MySpLight);

private:
    void UpdateCameraVectors();
};

#endif