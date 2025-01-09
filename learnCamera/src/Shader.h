#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Shader
{
public:
    Shader(const char* vertexPath,const char* fragmentPath);
    void use();
    void Shader::setBool(const std::string &name, bool value) const;
    void Shader::setInt(const std::string &name, int value) const;
    void Shader::setFloat(const std::string &name, float value) const;
public:
    unsigned int ID;        //shader program ID
    const char* vertexSource;
    const char* fragmentSource;    
private:
    void loadFile(const char* vertexPath,const char* fragmentPath);
    void checkCompileErrors(unsigned int errID,std::string type);
    void shaderCompile();
};

#endif