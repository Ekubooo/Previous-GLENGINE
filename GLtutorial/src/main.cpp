#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 定义-----------------------------------
    // 三角形顶点属性：(位置(x,y,z),颜色(r,g,b))
    // 逆时针绘制*, 顶点间fragmentShader会自动插值*
float vertices[] = 
{   // 第一个三角 
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   //0 
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   //1
     0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,   //2
     // 第二个三角形(复用了两个坐标)
  /* 0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f, */
     0.5f,  0.5f, 0.0f, 0.3f, 0.6f, 0.9f    //3
};
    // EBO 索引
unsigned int indices[] = 
{   0, 1, 2,
    2, 1, 3 };

// 函数注册-----------------------------------
    // 回调函数
void framebuffer_size_callback(GLFWwindow* window,int width,int height);
    // 输入检测
void processInput(GLFWwindow *window);

// shader-----------------------------------
// vertexShader一定会输出gl_Position， fragmentShader一定输出FragColor
// 在管线中shader输出的东西会被用于下一个阶段的shader输入，直到输出rgba到屏幕上
const char* vertexShaderSource = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec4 vertexColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   vertexColor = vec4(aColor.x,aColor.y,aColor.z,1.0f);\n"     
    "}\0";  // 在vertexShader中多传一个颜色下去    
const char *fragmentShaderSource = 
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec4 vertexColor;\n"
    "uniform vec4 ourColor; \n"          // 使用 uniform 全局数据
    "void main()\n"
    "{\n"
  //"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "   FragColor = vertexColor;\n"    
    "}\n\0";

// -----------------------------------
// 创建窗口：使用3.3版本 并 启用核心模式
void my_createWindow()
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
GLFWwindow* my_createWindowEntity()
{
    GLFWwindow* window = 
        glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT,"myWindow",NULL,NULL);
    if (window == NULL)
    {
        cout<<"Filed to create window"<<endl;
        glfwTerminate();        // 窗口终止
        return nullptr;
    }
    // 上下文切换
    glfwMakeContextCurrent(window);
    // 注册回调函数处
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
}

// -----------------------------------
// glad初始化
int my_gladLoader()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout<<"Failed to initialize GLAD"<<endl;
        glfwTerminate();
        return -1;
    }
    else return 100;
}

// -----------------------------------
// 创建视口，创建回调函数进行反应 
void my_viewPort()
{
    glViewport(0,0,SCR_WIDTH,SCR_HEIGHT);
    // 启用剔除
/*  glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);    */
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    // 线框绘制模式
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void framebuffer_size_callback
(GLFWwindow* window,int width,int height)
{
    glViewport(0,0,width,height);
}

// -----------------------------------
// 渲染循环 交换双缓冲并执行绘制
// 
void my_renderLoop
(GLFWwindow* window, unsigned int& VAO, unsigned int& VBO,
unsigned int& EBO, unsigned int& shaderProgram)
{
    cout<<"statr render loop"<<endl;
    while (!glfwWindowShouldClose(window))
    {   
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgram);

        // 设定uniform全局变量:变化色(启用请在fragmentShader指定输出)
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue)/ 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram,"ourColor");
        glUniform4f(vertexColorLocation,0.0f,greenValue,0.0f,1.0f);

        // 绑定缓冲与着色器
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        //glDrawArrays(GL_TRIANGLES,0,6);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    cout<<"end render loop"<<endl;
}

// -----------------------------------
// 创建VAO  glGenBuffers可以产生多个VAO 即"buffers" 第二个参数可以是数组
// 
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
// 加载着色器：创建两个着色器，创建着色程式，依附shader并连接着色程式
void my_shaderLoader
(unsigned int& vertexShader,unsigned int& fragmentShader,unsigned int& shaderProgram)
{
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexShaderSource,nullptr);
    glCompileShader(vertexShader);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource,nullptr);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);
    
    // 顶点属性中加入了颜色属性
    // 属性参数:(location,维度大小,数据类型,标准化选项,步长,偏移量)
    glVertexAttribPointer
        (0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer   //rgb值位: location*偏移量, 所以location = 1 
        (1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
}

// 主程-----------------------------------
int main()
{
    unsigned int VAO = 1, VBO = 1,EBO = 1;
    unsigned int vertexShader,fragmentShader,shaderProgram;
    
    my_createWindow();                      // 创建窗口初始化

    GLFWwindow* window = my_createWindowEntity();
    if(window == nullptr) return -1;        // 创建窗口对象

    if(my_gladLoader() == -1) return -1;    // 初始化GLAD
    my_viewPort();                          // 创建视口，创建回调函数进行反应

    my_ContainerObjcet(VAO,VBO,EBO);        // 创建VAO, VBO, EBO 并绑定
    my_shaderLoader(vertexShader,fragmentShader,shaderProgram);
                                            // 创建并加载着色器
    my_renderLoop(window,VAO,VBO,EBO,shaderProgram);
                                            // 渲染循环
    glfwTerminate();
    return 0;
}

// -----------------------------------
// 输入检测
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    // 尝试：通过任意的按键事件改变颜色(GLFW_KEY_*)
    /* if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    if(glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }  */
    
}