#include "Shader.h"

using namespace std;

Shader::Shader(const char* vertexPath,const char* fragmentPath)
{
    ifstream vertexFile; 
    ifstream fragmentFile; 
    stringstream vertexSStream;
    stringstream fragmentSStream;

    vertexFile.open(vertexPath);
    fragmentFile.open(fragmentPath);
    vertexFile.exceptions(ifstream::failbit || ifstream::badbit);
    fragmentFile.exceptions(ifstream::failbit || ifstream::badbit);

    try
    {
        if(!vertexFile.is_open() || !fragmentFile.is_open())
            {throw std::exception("---open file error---");}

        vertexSStream << vertexFile.rdbuf();
        fragmentSStream << fragmentFile.rdbuf();

        vertexString = vertexSStream.str();
        fragmentString = fragmentSStream.str();
        vertexSource = vertexString.c_str();
        fragmentSource = fragmentString.c_str(); 
        /*  
        cout<<"shader file:"<<endl
            <<vertexSource<<endl<<fragmentSource<<endl; 
        */
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

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    catch(const std::exception& ex)
    {
        std::cerr << ex.what() << '\n';
    }  
       
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
