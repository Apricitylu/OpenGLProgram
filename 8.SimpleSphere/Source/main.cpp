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
#include <vector>

#include "../../Common/utils.h"
#include "../../Common/Sphere.h"


using namespace std;

#define numVAOs 1
#define numVBOs 3

float cameraX, cameraY, cameraZ;
float LocX, LocY, LocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// 分配在 display() 函数中使用的变量空间，这样它们就不必在渲染过程中分配
GLuint mLoc, vLoc, mvLoc, projLoc, tfLoc;
int width, height;
float aspect, timeFactor;
glm::mat4 pMat, vMat, mMat, mvMat, tMat, rMat;

Sphere mySphere(12);

void setupVertices(void) {    // 36个顶点，12个三角形，组成了放置在原点处的2×2×2立方体
    std::vector<int> ind = mySphere.getIndices();
    std::vector<glm::vec3> vert = mySphere.getVertices();
    std::vector<glm::vec2> tex = mySphere.getTexCoords();
    std::vector<glm::vec3> norm = mySphere.getNormals();

    std::vector<float> pvalues;     // 顶点位置
    std::vector<float> tvalues;     // 纹理坐标
    std::vector<float> nvalues;     // 法向量

    int numIndices = mySphere.getNumIndices();
    for (int i = 0; i < numIndices; i++) {
        pvalues.push_back((vert[ind[i]]).x);
        pvalues.push_back((vert[ind[i]]).y);
        pvalues.push_back((vert[ind[i]]).z);

        tvalues.push_back((tex[ind[i]]).s);
        tvalues.push_back((tex[ind[i]]).t);

        nvalues.push_back((norm[ind[i]]).x);
        nvalues.push_back((norm[ind[i]]).y);
        nvalues.push_back((norm[ind[i]]).z);
    }

    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(3, vbo);

    // 把顶点放入缓冲区 #0
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

    // 把纹理坐标放入缓冲区 #1
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

    // 把法向量放入缓冲区 #2
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);
}

void init(GLFWwindow* window) {
    const char* vp = "Source/vertShader.glsl";
    const char* fp = "Source/fragShader.glsl";
    renderingProgram = Utils::createShaderProgram(vp, fp);
    cameraX = 0.0f; cameraY = 0.0f; cameraZ = 3.0f;
    LocX = 0.0f; LocY = 0.0f; LocZ = 0.0f; // 沿Y轴下移以展示透视
    setupVertices();
}

void display(GLFWwindow* window, double currentTime) {
    glClear(GL_DEPTH_BUFFER_BIT);//清除深度缓冲区
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renderingProgram);

    // 获取MV矩阵和投影矩阵的统一变量
    mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
    projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

    // 构建透视矩阵
    glfwGetFramebufferSize(window, &width, &height);
    aspect = (float)width / (float)height;
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees

    // 构建视图矩阵、模型矩阵和视图-模型矩阵
    vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(LocX, LocY, LocZ));
    mMat *= glm::rotate(glm::mat4(1.0f), (float)currentTime * 0.1f, glm::vec3(1.0, 0.0, 0.0));
    mvMat = vMat * mMat;

    // 将透视矩阵和MV矩阵复制给相应的统一变量
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

    // 将VBO关联给顶点着色器中相应的顶点属性
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // 调整OpenGL设置，绘制模型
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//呈现线框模型
    glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());
}

int main(void) {                            // main()和之前的没有变化
    if (!glfwInit()) { exit(EXIT_FAILURE); }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(600, 600, "SphereDemo", NULL, NULL);
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