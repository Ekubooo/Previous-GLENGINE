#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    unsigned int ID;        //shader program ID   
    enum slot
    {
        DIFFUSE,
        SPECULAR
    };

public:
    Shader(const char* vertexPath,const char* fragmentPath);
    ~Shader();
    void use();
    void Shader::setBool(const std::string &name, bool value) const;
    void Shader::setInt(const std::string &name, int value) const;
    void Shader::setFloat(const std::string &name, float value) const;
    void setUniform3f(const char* paramNameString, glm::vec3 param);
    void serUniform1Img(const char* paramNameString, int slot);
private:
    const char* vertexSource;
    const char* fragmentSource;
    std::string vertexString;
    std::string fragmentString;
private:
    void loadFile(const char* vertexPath,const char* fragmentPath);
    void checkCompileErrors(unsigned int errID,std::string type);
    void shaderCompile();
};

#endif