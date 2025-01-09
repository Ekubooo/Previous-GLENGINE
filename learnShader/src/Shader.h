#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Shader
{
public:
    unsigned int ID;        //shader program ID
    std::string vertexString;
    std::string fragmentString;
    const char* vertexSource;
    const char* fragmentSource;
public:
    Shader(const char* vertexPath,const char* fragmentPath );
    void use();
private:
    void checkCompileErrors(unsigned int errID,std::string type);
};

#endif