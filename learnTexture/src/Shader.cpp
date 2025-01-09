#include "Shader.h"
using namespace std;

Shader::Shader(const char* vertexPath,const char* fragmentPath)
{
    loadFile(vertexPath,fragmentPath);
    shaderCompile();
}
void Shader::loadFile(const char* vertexPath,const char* fragmentPath)
{
    std::string vertexString, fragmentString;
    ifstream vertexFile, fragmentFile;
    stringstream vertexSStream, fragmentSStream;

    vertexFile.open(vertexPath);
    fragmentFile.open(fragmentPath);
    vertexFile.exceptions(ifstream::failbit || ifstream::badbit);
    fragmentFile.exceptions(ifstream::failbit || ifstream::badbit);

    try
    {
        if(!vertexFile.is_open() || !fragmentFile.is_open())
        {
            throw std::exception("---open file error---");
        }
        vertexSStream << vertexFile.rdbuf();
        fragmentSStream << fragmentFile.rdbuf();

        vertexString = vertexSStream.str();
        fragmentString = fragmentSStream.str();
        vertexSource = vertexString.c_str();
        fragmentSource = fragmentString.c_str(); 
    }
    catch(const std::exception& ex)
    {
        std::cerr << ex.what() << '\n';
    }  
}

void Shader::shaderCompile()
{
    unsigned int vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSource, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex,"VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSource, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment,"FRAGMENT");
    
    ID = glCreateProgram();
    glAttachShader(ID,vertex);
    glAttachShader(ID,fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID,"PROGRAM");

    glDeleteShader(vertex);         // 用完记得删除
    glDeleteShader(fragment);       // 用完记得删除
}

void Shader::use()
{
     glUseProgram(ID);
}
void Shader::checkCompileErrors(unsigned int ID,std::string type)
{
    int success;
    char infoLog[512];
    if (type != "PROGRAM")
    {
        glGetShaderiv(ID,GL_COMPILE_STATUS,&success);
        if(!success)
        {
            glGetShaderInfoLog(ID, 512, nullptr, infoLog);
            cout<<"shader compile error:"<<infoLog<<endl;
        } 
    }
    else
    {
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(ID, 512, nullptr, infoLog);
            cout<<"program linking error:"<<infoLog<<endl;
        }
    }
}
void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
}
void Shader::setInt(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}
void Shader::setFloat(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
} 
