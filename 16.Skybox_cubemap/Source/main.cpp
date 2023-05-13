#pragma once

//区分Debug模式和Release模式
#ifdef _DEBUG
#pragma comment(lib,"../../Debug/Common.lib")
#else
#pragma comment(lib,"../../Release/Common.lib")
#endif

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <stack>

#include "../../Common/utils.h"

using namespace std;

#define numVAOs 1
#define numVBOs 4

float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;
float pyramidLocX, pyramidLocY, pyramidLocZ;
GLuint renderingProgram, renderingProgramCubeMap;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

GLuint skyboxTexture;
GLuint brickTexture;

// 分配在 display() 函数中使用的变量空间，这样它们就不必在渲染过程中分配
GLuint mLoc, vLoc, mvLoc, projLoc, tfLoc;
int width, height;
float aspect, timeFactor;
glm::mat4 pMat, vMat, mMat, mvMat, tMat, rMat;
stack<glm::mat4> mvStack;

void setupVertices(void) {
    float skyboxPositions[108] = {
       -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
       1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
       1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
       1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
       -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
       -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
       -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
       -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
       -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
       1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f
    };

    float skyboxTextureCoord[72] = {
     1.00f, 0.66f, 1.00f, 0.33f, 0.75f, 0.33f,    // 背面右下角
     0.75f, 0.33f, 0.75f, 0.66f, 1.00f, 0.66f,    // 背面左上角
     0.75f, 0.33f, 0.50f, 0.33f, 0.75f, 0.66f,    // 右面右下角
     0.50f, 0.33f, 0.50f, 0.66f, 0.75f, 0.66f,    // 右面左上角
     0.50f, 0.33f, 0.25f, 0.33f, 0.50f, 0.66f,    // 正面右下角
     0.25f, 0.33f, 0.25f, 0.66f, 0.50f, 0.66f,    // 正面左上角
     0.25f, 0.33f, 0.00f, 0.33f, 0.25f, 0.66f,    // 左面右下角
     0.00f, 0.33f, 0.00f, 0.66f, 0.25f, 0.66f,    // 左面左上角
     0.25f, 0.33f, 0.50f, 0.33f, 0.50f, 0.00f,    // 下面右下角
     0.50f, 0.00f, 0.25f, 0.00f, 0.25f, 0.33f,    // 下面左上角
     0.25f, 1.00f, 0.50f, 1.00f, 0.50f, 0.66f,    // 上面右下角
     0.50f, 0.66f, 0.25f, 0.66f, 0.25f, 1.00f     // 上面左上角
    };

    float pyramidPositions[54] = {
        -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f
    };

    float pyrTexCoords[36] = {
        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,   // 前侧面
        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,   // 右侧面
        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,   // 后侧面
        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,   // 左侧面
        0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,   // 底面的三角形1
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f    // 底面的三角形2
    };

    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs, vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxPositions), skyboxPositions, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxTextureCoord), skyboxTextureCoord, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPositions), pyramidPositions, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyrTexCoords), pyrTexCoords, GL_STATIC_DRAW);
}

void init(GLFWwindow* window) {
    const char* vp = "Source/vertShader.glsl";
    const char* fp = "Source/fragShader.glsl";
    renderingProgram = Utils::createShaderProgram(vp, fp);
    const char* vcp = "Source/vertCShader.glsl";
    const char* fcp = "Source/fragCShader.glsl";
    renderingProgramCubeMap = Utils::createShaderProgram(vcp, fcp);

    cameraX = 0.0f; cameraY = 0.0f; cameraZ = 8.0f;
    pyramidLocX = 2.0f; pyramidLocY = 2.0f; pyramidLocZ = 0.0f; // 沿Y轴下移以展示透视

    brickTexture = Utils::loadTexture("brick1.jpg");

    skyboxTexture = Utils::loadCubeMap("skybox_cubemap");         // 包含天空盒纹理的文件夹
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);//混合立方体相邻的边以减少或消除接缝

    setupVertices();
}

void display(GLFWwindow* window, double currentTime) {
    glClear(GL_DEPTH_BUFFER_BIT);//清除深度缓冲区
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(renderingProgramCubeMap);

    // 构建透视矩阵
    glfwGetFramebufferSize(window, &width, &height);
    aspect = (float)width / (float)height;
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees

    //天空盒
    // 获取MV矩阵和投影矩阵的统一变量
    mvLoc = glGetUniformLocation(renderingProgramCubeMap, "v_matrix");
    projLoc = glGetUniformLocation(renderingProgramCubeMap, "proj_matrix");
   
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

    // 将视图矩阵推入堆栈
    vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

    // 准备首先绘制天空盒。M矩阵将天空盒放置在摄像机位置
    //mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cameraX, cameraY, cameraZ));

    // 构建MODEL-VIEW矩阵
    //mvMat = vMat * mMat;

    // 如前，将MV和PROJ矩阵放入统一变量
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(vMat));

    // 设置包含顶点的缓冲区
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // 设置包含纹理坐标的缓冲区
    //glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    //glEnableVertexAttribArray(1);

    //激活天空盒纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);    // 立方体缠绕顺序是顺时针的，但我们从内部查看，因此使用逆时针缠绕顺序GL_CCW

    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 36);       // 在没有深度测试的情况下绘制天空盒
    glEnable(GL_DEPTH_TEST);

    
    //物体
    glUseProgram(renderingProgram);

    // 获取MV矩阵和投影矩阵的统一变量
    mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
    projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    mMat *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.3f, 1.0f, 0.0f));
    mvMat = vMat * mMat;
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, brickTexture);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glDrawArrays(GL_TRIANGLES, 0, 18);
}

int main(void) {                            // main()和之前的没有变化
    if (!glfwInit()) { exit(EXIT_FAILURE); }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(600, 600, "Skybox", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
    glfwSwapInterval(1);

    init(window);

    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}