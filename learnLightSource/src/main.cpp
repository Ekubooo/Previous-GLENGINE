#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "Shader.h"
#include "Camera.h"
#include "Material.h"
#include "LightDirectional.h"
#include "LightPoint.h"
#include "LightSpot.h"

using namespace std;

// 全局变量-----------------------------------
#pragma region InputDeclare

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
const char* vertexPath = "assets/vertexShader.vert";
const char* fragmentPath = "assets/fragmentShader.frag";
unsigned int VAO = 1, VBO = 1,EBO = 1;
unsigned int myTexBufferA = 1, myTexBufferB = 1;

const glm::vec3 loc (0,0,3.0f);
const glm::vec3 gazeAt (0,-1.0f,0);
const glm::vec3 world_Z (0,1.0f,0);

const float pch = glm::radians(-10.0f);
const float yw = glm::radians(180.0f);
float lastX , lastY;
bool firstMouse = true;
// 光照
const glm::vec3 ambient(1.0f, 1.0f, 1.0f);
const unsigned int diffuse = 0;
const unsigned int specular = 0;
const float shininess = 64.0f;
unsigned int TexDiffuse = 0;
unsigned int TexSpecular = 0;

glm::mat4 trans,modelMat,modelMat2,viewMat,projMat; 
map<string ,glm::mat4> transCon = 
    {{"trans",trans}, {"modelMat",modelMat},{"modelMat2",modelMat2},
    {"viewMat",viewMat}, {"projMat",projMat}};
map<string,unsigned int> myTexBuffer = 
    {{"myTexBufferA", myTexBufferA},
     {"myTexBufferB", myTexBufferB},
     {"diffuse", diffuse},
     {"specular", specular}};

// 光源(平行光)
const glm::vec3 DLPostion (0.0f, 0.18f, 1.2f);
const glm::vec3 DLAngle (glm::radians(180.0f), 0, 0);
const glm::vec3 DLColor (2.0f, 2.0f, 2.0f);
const glm::vec3 Px(1.0f, 0, 0);
const glm::vec3 Py(0, 1.0f, 0);
const glm::vec3 Pz(0, 0, 1.0f);

#pragma endregion
// 对象指针-----------------------------------

Camera* my_Camera =                     // 创建相机 
    new Camera(loc,pch,yw,world_Z);     // 欧拉角方法
LightDirectional* MyDirLight =          // 光源(平行光)对象
    new LightDirectional(DLPostion, DLAngle/* , DLColor */);
LightPoint* MyP0Light =                 // 点光源
    new LightPoint(DLPostion, DLAngle/* , DLColor */);
LightPoint* MyP1Light =                 // 点光源
    new LightPoint(Px, DLAngle, Px);
LightPoint* MyP2Light =                 // 点光源
    new LightPoint(Py, DLAngle, Py);
LightPoint* MyP3Light =                 // 点光源
    new LightPoint(Pz, DLAngle, Pz);
LightSpot* MySpLight =                  // 手电筒光
    new LightSpot(DLPostion, DLAngle, DLColor);

// 模型数据-----------------------------------
    // 三角形顶点属性：(位置(x,y,z),颜色(r,g,b),uv(u,v))
    // 顶点逆时针绘制, 顶点间颜色fragmentShader会自动插值*
#pragma region ModelData 
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    // EBO 索引
unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0 };

glm::vec3 cubePositions[] = {
  glm::vec3( 0.0f,  0.0f,  0.0f), 
  glm::vec3( 2.0f,  5.0f, -15.0f), 
  glm::vec3(-1.5f, -2.2f, -2.5f),  
  glm::vec3(-3.8f, -2.0f, -12.3f),  
  glm::vec3( 2.4f, -0.4f, -3.5f),  
  glm::vec3(-1.7f,  3.0f, -7.5f),  
  glm::vec3( 1.3f, -2.0f, -2.5f),  
  glm::vec3( 1.5f,  2.0f, -2.5f), 
  glm::vec3( 1.5f,  0.2f, -1.5f), 
  glm::vec3(-1.3f,  1.0f, -1.5f)  
};
#pragma endregion

// 函数注册-----------------------------------
    // 回调函数
void framebuffer_size_callback(GLFWwindow* window,int width,int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    // 输入检测
void processInput(GLFWwindow *window);
void glmTest();

// -----------------------------------
// 创建窗口：使用3.3版本 并 启用核心模式
void my_InitWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);   //主版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);   //次版本
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    //macOS配置
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
}

// -----------------------------------
// 创建窗口对象(实体)
GLFWwindow* my_createWindow()
{
    GLFWwindow* window = glfwCreateWindow
        (SCR_WIDTH,SCR_HEIGHT,"myWindow",nullptr,nullptr);    
    if (window == nullptr)
    {
        cout<<"Filed to create window"<<endl;
        glfwTerminate();        // 窗口终止
        return nullptr;
    }
    // 上下文切换
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);    // 鼠标失焦
    glfwSetCursorPosCallback(window, mouse_callback);
    // 注册回调函数处
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
}

// -----------------------------------
// glad初始化
void my_gladLoader()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout<<"Failed to initialize GLAD"<<endl;
        glfwTerminate();
        return;
    }
}

// -----------------------------------
// 创建视口，创建回调函数进行反应 
void my_viewPort()
{
    glViewport(0,0,SCR_WIDTH,SCR_HEIGHT);
     
    //glEnable(GL_CULL_FACE);     // 启用剔除    
    //glCullFace(GL_BACK);         // 裁切背面(注意绘制逆时针与正反面)
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    // 线框绘制模式
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);                        // 启用Z-Buffer
}

// -----------------------------------
// 创建VAO  glGenBuffers可以产生多个VAO 即"buffers" 第二个参数可以是数组
void my_ContainerObjcet
(unsigned int& VAO,unsigned int& VBO,unsigned int& EBO)
{
    glGenVertexArrays(1,&VAO); 
    glBindVertexArray(VAO);

    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData
        (GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData
        (GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
}

// -----------------------------------
// 顶点属性中加入了颜色属性 加入了uv值
// 属性参数:(location,维度(数量),数据类型,标准化选项,步长,偏移量)
void my_VertexAttribute()
{
    glVertexAttribPointer
        (0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer   //rgb值位: location*偏移量, 所以location = 1 
        (1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1); 

    glVertexAttribPointer   //uv参数: location*偏移量, 所以location = 1 
        (2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
}
void my_cubeVertexAttribute()
{   //使用 drawArray
    glVertexAttribPointer
        (0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer   //uv参数: location*偏移量, 所以location = 1 
        (2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(2);
}
void my_lightVertexAttribute()
{
    glVertexAttribPointer
        (0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer    
        (1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer    
        (2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
}

// -----------------------------------
// 读取器:读取纹理(适配不同通道: RGB or RGBA)
// 返回值是否改为uint ?
void GPUloadImageRGBA
    (unsigned int& myTexBuffer, const char* myTextureImage)
{
    stbi_set_flip_vertically_on_load(true);
    int width, hight, nrChannel;
    unsigned char* data = 
    stbi_load(myTextureImage, &width, &hight, &nrChannel, 0);
    glGenTextures(1, &myTexBuffer);
    //glActiveTexture(GL_TEXTURE0);   // 启用0号通道(共16个)
    glBindTexture(GL_TEXTURE_2D, myTexBuffer); 
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, hight,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else 
        {cout<<"load image RGBA failed!!"<<endl;}
    stbi_image_free(data);
}
unsigned int GPULoadImage
    (const char* filename,GLint iFormat,GLenum format, int texSlot)
{
    unsigned int myTexBuffer;
    int width, hight, nrChannel;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = 
        stbi_load(filename, &width, &hight, &nrChannel, 0);
    glGenTextures(1, &myTexBuffer);
    glActiveTexture(GL_TEXTURE0 + texSlot);   // 启用slot号通道
    glBindTexture(GL_TEXTURE_2D, myTexBuffer); 
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, iFormat, width, hight,
                     0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else 
        {cout<<"load image RGBA failed!!"<<endl;}
    stbi_image_free(data);
    return myTexBuffer;
}

// -----------------------------------
// 纹理加载器
void my_textureLoader
    (map<string,unsigned int>& myTexBuffer)
{
    const char* myTextureImage1 = "assets/my_icon/container.jpg";
    const char* myTextureImage2 = "assets/my_icon/AstroTexture.png";
    const char* myTextureImage3 = "assets/my_icon/awesomeface.png";
    const char* myTextureImage4 = "assets/my_icon/container2.png";
    const char* myTextureImage5 = "assets/my_icon/container2_specular.png";
    GPUloadImageRGBA(myTexBuffer["myTexBufferA"], myTextureImage3);
    GPUloadImageRGBA(myTexBuffer["myTexBufferB"], myTextureImage2);
    GPUloadImageRGBA(myTexBuffer["diffuse"], myTextureImage4);
    GPUloadImageRGBA(myTexBuffer["specular"], myTextureImage5);
    
}
void my_textureLoader
    (unsigned int& TexDiffuse,unsigned int& TexSpecular)
{
    const char* myTextureImage1 = "assets/my_icon/container.jpg";
    const char* myTextureImage2 = "assets/my_icon/AstroTexture.png";
    const char* myTextureImage3 = "assets/my_icon/awesomeface.png";
    const char* myTextureImage4 = "assets/my_icon/container2.png";
    const char* myTextureImage5 = "assets/my_icon/container2_specular.png";
    TexDiffuse = GPULoadImage(myTextureImage4,GL_RGBA,GL_RGBA,Shader::DIFFUSE);
    TexSpecular = GPULoadImage(myTextureImage5,GL_RGBA,GL_RGBA,Shader::SPECULAR);
}

// -----------------------------------
// 纹理激活器(失效中) 被GPULoadImage取代功能
void my_textureActivator
(Shader* myShader, Material* myMaterial, map<string,unsigned int>& myTexBuffer)
{
    glActiveTexture(GL_TEXTURE0);   // 启用0号通道(共16个)
    glBindTexture(GL_TEXTURE_2D, myMaterial->_diffuse);
    glActiveTexture(GL_TEXTURE1);   // 启用1号通道(共16个)
    glBindTexture(GL_TEXTURE_2D, myMaterial->_specular); 
}

// -----------------------------------
// 变换矩阵 (在此计算矩阵变换) 变换顺序是逐变换矩阵右乘在原矩阵上, 注意顺序
// MVP变换,使用了openGL包装的变换函数
void my_transMit(map<string ,glm::mat4>& transCon, Camera* my_Camera)
{   // MVP
    float WofH = (float)SCR_WIDTH/(float)SCR_HEIGHT;    // 宽高比
    transCon["modelMat"] = 
        glm::rotate(transCon["modelMat"], glm::radians(-35.0f),glm::vec3(1.0f,0,0));
    transCon["viewMat"] =                               // 替换为相机类
        glm::translate(transCon["viewMat"], glm::vec3(0,0,-3.0f));
    //transCon["viewMat"] = my_Camera->getViewMatrix(); // 替换为相机类
    transCon["projMat"] =       // perspective(fov, 宽高比, 近平面, 远平面)
        glm::perspective(glm::radians(45.0f), WofH, 0.1f,100.0f);
}

// -----------------------------------
// 循环内变换 即 实时变换
void inLoop_transMit(map<string ,glm::mat4>& transCon)
{
    glm::mat4 nothing;  // 转速归零，否则转动量叠加，旋转会加速
    transCon["modelMat"] = nothing;
    transCon["modelMat"] = glm::rotate(transCon["modelMat"], 
                (float)glfwGetTime() * glm::radians(35.0f), 
                glm::vec3(0.5f, 1.0f, 0.0f));
}

// -----------------------------------
// uniforme全局变量设定
// glUniform4f(目标位置,四通道vec4(RGBA)) :此时shader中uniform为vce4f
void my_uniformSetting
    (Shader* myShader,Material* myMaterial,map<string,glm::mat4>& transCon)
{   
    // MVP uniform 传递
    glUniformMatrix4fv(glGetUniformLocation(myShader->ID,"modelMat"), 
        1, GL_FALSE, glm::value_ptr(transCon["modelMat2"]));
    glUniformMatrix4fv(glGetUniformLocation(myShader->ID,"viewMat"), 
        1, GL_FALSE, glm::value_ptr(transCon["viewMat"]));
    glUniformMatrix4fv(glGetUniformLocation(myShader->ID,"projMat"), 
        1, GL_FALSE, glm::value_ptr(transCon["projMat"]));
    // 光照uniform    
    glUniform3f(glGetUniformLocation(myShader->ID,"objColor"),1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(myShader->ID,"ambientColor"),0.5f, 0.5f, 0.5f);
    // dir Light
    glUniform3f(glGetUniformLocation(myShader->ID,"ltDir.pos"),
        MyDirLight->_position.x, MyDirLight->_position.y, MyDirLight->_position.z);
    glUniform3f(glGetUniformLocation(myShader->ID,"ltDir.color"),
        MyDirLight->_color.x, MyDirLight->_color.y, MyDirLight->_color.z);
    glUniform3f(glGetUniformLocation(myShader->ID,"ltDir.dir2Light"),
        MyDirLight->_directoion.x, MyDirLight->_directoion.y, MyDirLight->_directoion.z);
    // point Light
    // 0---------------------------------
    glUniform3f(glGetUniformLocation(myShader->ID,"lightP0.pos"),
        MyP0Light->_position.x, MyP0Light->_position.y, MyP0Light->_position.z);
    glUniform3f(glGetUniformLocation(myShader->ID,"lightP0.color"),
        MyP0Light->_color.x, MyP0Light->_color.y, MyP0Light->_color.z);
    glUniform3f(glGetUniformLocation(myShader->ID,"lightP0.dir2Light"),
        MyP0Light->_directoion.x, MyP0Light->_directoion.y, MyP0Light->_directoion.z);  
    // 衰减值
    glUniform1f(glGetUniformLocation(myShader->ID,"lightP0.constant"),MyP0Light->_constant);
    glUniform1f(glGetUniformLocation(myShader->ID,"lightP0.linear"),MyP0Light->_linear);
    glUniform1f(glGetUniformLocation(myShader->ID,"lightP0.quadratic"),MyP0Light->_quadratic);
    // 1---------------------------------
    glUniform3f(glGetUniformLocation(myShader->ID,"lightP1.pos"),
        MyP1Light->_position.x, MyP1Light->_position.y, MyP1Light->_position.z);
    glUniform3f(glGetUniformLocation(myShader->ID,"lightP1.color"),
        MyP1Light->_color.x, MyP1Light->_color.y, MyP1Light->_color.z);
    glUniform3f(glGetUniformLocation(myShader->ID,"lightP1.dir2Light"),
        MyP1Light->_directoion.x, MyP1Light->_directoion.y, MyP1Light->_directoion.z);  
    // 衰减值
    glUniform1f(glGetUniformLocation(myShader->ID,"lightP1.constant"),MyP1Light->_constant);
    glUniform1f(glGetUniformLocation(myShader->ID,"lightP1.linear"),MyP1Light->_linear);
    glUniform1f(glGetUniformLocation(myShader->ID,"lightP1.quadratic"),MyP1Light->_quadratic);
    // 2---------------------------------
    glUniform3f(glGetUniformLocation(myShader->ID,"lightP2.pos"),
        MyP2Light->_position.x, MyP2Light->_position.y, MyP2Light->_position.z);
    glUniform3f(glGetUniformLocation(myShader->ID,"lightP2.color"),
        MyP2Light->_color.x, MyP2Light->_color.y, MyP2Light->_color.z);
    glUniform3f(glGetUniformLocation(myShader->ID,"lightP2.dir2Light"),
        MyP2Light->_directoion.x, MyP2Light->_directoion.y, MyP2Light->_directoion.z);  
    // 衰减值
    glUniform1f(glGetUniformLocation(myShader->ID,"lightP2.constant"),MyP2Light->_constant);
    glUniform1f(glGetUniformLocation(myShader->ID,"lightP2.linear"),MyP2Light->_linear);
    glUniform1f(glGetUniformLocation(myShader->ID,"lightP2.quadratic"),MyP2Light->_quadratic);
    // 3---------------------------------
    glUniform3f(glGetUniformLocation(myShader->ID,"lightP3.pos"),
        MyP3Light->_position.x, MyP3Light->_position.y, MyP3Light->_position.z);
    glUniform3f(glGetUniformLocation(myShader->ID,"lightP3.color"),
        MyP3Light->_color.x, MyP3Light->_color.y, MyP3Light->_color.z);
    glUniform3f(glGetUniformLocation(myShader->ID,"lightP3.dir2Light"),
        MyP3Light->_directoion.x, MyP3Light->_directoion.y, MyP3Light->_directoion.z);  
    // 衰减值
    glUniform1f(glGetUniformLocation(myShader->ID,"lightP3.constant"),MyP3Light->_constant);
    glUniform1f(glGetUniformLocation(myShader->ID,"lightP3.linear"),MyP3Light->_linear);
    glUniform1f(glGetUniformLocation(myShader->ID,"lightP3.quadratic"),MyP3Light->_quadratic);
    // end-------------------------------

    // spot
    glUniform3f(glGetUniformLocation(myShader->ID,"ltSpot.pos"),
        MySpLight->_position.x, MySpLight->_position.y, MySpLight->_position.z);
    glUniform3f(glGetUniformLocation(myShader->ID,"ltSpot.color"),
        MySpLight->_color.x, MySpLight->_color.y, MySpLight->_color.z);
    glUniform3f(glGetUniformLocation(myShader->ID,"ltSpot.dir2Light"),
        MySpLight->_directoion.x, MySpLight->_directoion.y, MySpLight->_directoion.z);  
    // spot 渐变参数
    glUniform1f(glGetUniformLocation(myShader->ID,"ltSpot.cosPhiInner"),MySpLight->_cosPhiInner);
    glUniform1f(glGetUniformLocation(myShader->ID,"ltSpot.cosPhiOuter"),MySpLight->_cosPhiOuter);

    // camera 
    glUniform3f(glGetUniformLocation(myShader->ID,"cameraPos"),
        my_Camera->_Position.x, my_Camera->_Position.y, my_Camera->_Position.z);
    
    // material uniform 使用了shader类封装的传递Uniform方法
    myMaterial->_myShader->setUniform3f("material.ambient",myMaterial->_ambient);
    myMaterial->_myShader->serUniform1Img("material.diffuse", Shader::DIFFUSE);
    myMaterial->_myShader->serUniform1Img("material.specular", Shader::SPECULAR);
    // 0号参考GPUloadImageRGBA 方法
    myMaterial->_myShader->setFloat("material.shininess",myMaterial->_shininess);
}

// -----------------------------------
// darwCall多次调用 (如何只drawArray一次就可以画10个cube? 有关bench)*******
void my_drawCall
    (map<string ,glm::mat4>& transCon, Shader* myShader,Material* myMaterial)
{
    for (size_t i = 0; i < 10; i++)
    {   
        glm::mat4 modelMat2;
        transCon["modelMat2"] = modelMat2; 
        transCon["modelMat2"] = 
            glm::translate(transCon["modelMat2"], cubePositions[i]);
        float angle = 20.0f * i; 
        transCon["modelMat2"] = glm::rotate(transCon["modelMat2"], 
                    (float)glfwGetTime() * glm::radians(angle), 
                    glm::vec3(1.0f, 0.3f, 0.5f));
        my_uniformSetting(myShader,myMaterial,transCon);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

// -----------------------------------
//渲染循环
void my_renderLoop
(GLFWwindow* window, Shader* myShader, Camera* my_Camera, Material* myMaterial,
 unsigned int& VAO, unsigned int& EBO, 
 map<string ,glm::mat4>& transCon, map<string,unsigned int>& myTexBuffer)
{
    cout<<"statr render loop"<<endl;
    while (!glfwWindowShouldClose(window))
    {   
        //inLoop_transMit(transCon);            // loop内变换
        processInput(window);                   // 输入检测
        glClearColor(0.6f, 1.0f, 1.0f, 1.0f);   //(0.2f, 0.3f, 0.3f, 1.0f)
        // 每帧清除flag: 颜色缓存 深度缓存(Z-Buffer)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        
        myShader->use(); 
        // 绑定: 缓冲 着色器 纹理 
        //my_textureActivator(myShader,myMaterial,myTexBuffer);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
        transCon["viewMat"] = my_Camera->getViewMatrix();
        my_drawCall(transCon,myShader,myMaterial);     // uniform + drawArray 10Times
        glfwSwapBuffers(window);
        glfwPollEvents();
        my_Camera->updateCameraPos();
    }
    cout<<"end render loop"<<endl;
}

// 主程 -------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////
int main()
{   
    my_InitWindow();                        // 窗口初始化
    GLFWwindow* window = my_createWindow(); // 创建窗口对象
         
    my_gladLoader();                        // 初始化 GLAD库
    my_viewPort();                          // 创建视口，创建回调函数进行反应
    my_ContainerObjcet(VAO,VBO,EBO);        // 创建VAO, VBO, EBO 并绑定
    Shader* myShader = new Shader           // 初始化着色器类,创建&加载着色器
        (vertexPath,fragmentPath);
                   
    my_lightVertexAttribute();              // 设定顶点属性
    //my_textureLoader(myTexBuffer);        // 加载纹理
    my_textureLoader(TexDiffuse, TexSpecular);
    Material* myMaterial = new Material     // 初始化材质类
        (myShader,TexDiffuse, TexSpecular, ambient, shininess); 
    my_transMit(transCon,my_Camera);        // 矩阵变换 MVP
    my_renderLoop                           // 渲染循环
        (window,myShader,my_Camera,myMaterial,VAO,EBO,transCon,myTexBuffer); 
    
    glfwTerminate();
    return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////
// -----------------------------------
// 输入检测
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        my_Camera->_SpeedZ = 1.0f;
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        my_Camera->_SpeedZ = -1.0f;
    else
        my_Camera->_SpeedZ = 0.0f;
    // ======================
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        my_Camera->_SpeedX = -1.0f;
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        my_Camera->_SpeedX = 1.0f;
    else 
        my_Camera->_SpeedX = 0.0f;
    // ======================
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        my_Camera->_SpeedY = 1.0f;
    else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        my_Camera->_SpeedY = -1.0f;
    else 
        my_Camera->_SpeedY = 0.0f;
}
void framebuffer_size_callback
    (GLFWwindow* window,int width,int height)
{
    glViewport(0,0,width,height);
}

void mouse_callback
    (GLFWwindow* window, double xpos, double ypos)
{
    float deltaX, deltaY;
    if (firstMouse == true)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    deltaX = xpos - lastX;
    deltaY = ypos - lastY;
    lastX = xpos;
    lastY = ypos;
    my_Camera->ProcessMouseMovement(deltaX, deltaY);
}

// -----------------------------------
// 测试函数
void testTexture()
{
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = 
        stbi_load("assets/AstroTexture.jpg", &width, &height, &nrChannels, 0);
    for (size_t i = 0; i < 10; i++)
    {
        cout<< (int)data[i]<< endl;
    }
    stbi_image_free(data);
 }
void glmTest()
{
    glm::vec4 myvec(1.0f,0,0,1.0f);
    glm::mat4 mytrans;  // 0.98版本默认矩阵为单位矩阵E
    mytrans = glm::translate(mytrans,glm::vec3(2.0f, 0, 0));
    myvec = mytrans * myvec;
    cout<<"("<<myvec.x<<" "<<myvec.y<<" "<<myvec.z<<")"<<endl;
}

// end --