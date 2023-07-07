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
#define numVBOs 4

float cameraX, cameraY, cameraZ;
float LocX, LocY, LocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

GLuint TextureID;
GLuint NormalTextureID;

// 分配在 display() 函数中使用的变量空间，这样它们就不必在渲染过程中分配
GLuint mLoc, vLoc, mvLoc, projLoc, tfLoc, nLoc;
int width, height;
float aspect, timeFactor;
glm::mat4 pMat, vMat, mMat, mvMat, invTrMat;

// 着色器统一变量中的位置
GLuint globalAmbLoc, ambLoc, diffLoc, specLoc, posLoc, mAmbLoc, mDiffLoc, mSpecLoc, mShiLoc;

glm::vec3 currentLightPos, lightPosV;   // 在模型和视觉空间中的光照位置，Vector3f类型
float lightPos[3];                      // 光照位置的浮点数组

// 初始化光照位置
glm::vec3 initialLightLoc = glm::vec3(5.0f, 2.0f, 2.0f);

// 白光特性
float globalAmbient[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
float lightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

// 黄金材质特性
float* matAmb = Utils::goldAmbient();
float* matDif = Utils::goldDiffuse();
float* matSpe = Utils::goldSpecular();
float matShi = Utils::goldShininess();

Sphere mySphere(48);

void setupVertices(void) {    // 36个顶点，12个三角形，组成了放置在原点处的2×2×2立方体
    std::vector<int> ind = mySphere.getIndices();
    std::vector<glm::vec3> vert = mySphere.getVertices();
    std::vector<glm::vec2> tex = mySphere.getTexCoords();
    std::vector<glm::vec3> norm = mySphere.getNormals();
    std::vector<glm::vec3> tang = mySphere.getTangents();

    std::vector<float> pvalues;     // 顶点位置
    std::vector<float> tvalues;     // 纹理坐标
    std::vector<float> nvalues;     // 法向量
    std::vector<float> tavalues;    // 切向量

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

        tavalues.push_back((tang[ind[i]]).x);
        tavalues.push_back((tang[ind[i]]).y);
        tavalues.push_back((tang[ind[i]]).z);
    }

    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs, vbo);

    // 把顶点放入缓冲区 #0
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

    // 把纹理坐标放入缓冲区 #1
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

    // 把法向量放入缓冲区 #2
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

    // 把切向量放入缓冲区 #3
    glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
    glBufferData(GL_ARRAY_BUFFER, tavalues.size() * 4, &tavalues[0], GL_STATIC_DRAW);
}

void init(GLFWwindow* window) {
    const char* vp = "Source/vertShader.glsl";
    const char* fp = "Source/fragShader.glsl";
    renderingProgram = Utils::createShaderProgram(vp, fp);
    cameraX = 0.0f; cameraY = 0.0f; cameraZ = 3.0f;
    LocX = 0.0f; LocY = 0.0f; LocZ = 0.0f; // 沿Y轴下移以展示透视

    TextureID = Utils::loadTexture("Sphere.jpg");
    NormalTextureID = Utils::loadTexture("Normal.jpg");

    setupVertices();
}

void installLights(glm::mat4 vMatrix) {
    // 将光源位置转换为视图空间坐标，并存入浮点数组
    lightPosV = glm::vec3(vMatrix * glm::vec4(currentLightPos, 1.0));
    lightPos[0] = lightPosV.x;
    lightPos[1] = lightPosV.y;
    lightPos[2] = lightPosV.z;

    // 在着色器中获取光源位置和材质属性
    globalAmbLoc = glGetUniformLocation(renderingProgram, "globalAmbient");
    ambLoc = glGetUniformLocation(renderingProgram, "light.ambient");
    diffLoc = glGetUniformLocation(renderingProgram, "light.diffuse");
    specLoc = glGetUniformLocation(renderingProgram, "light.specular");
    posLoc = glGetUniformLocation(renderingProgram, "light.position");
    mAmbLoc = glGetUniformLocation(renderingProgram, "material.ambient");
    mDiffLoc = glGetUniformLocation(renderingProgram, "material.diffuse");
    mSpecLoc = glGetUniformLocation(renderingProgram, "material.specular");
    mShiLoc = glGetUniformLocation(renderingProgram, "material.shininess");

    // 在着色器中为光源与材质统一变量赋值
    glProgramUniform4fv(renderingProgram, globalAmbLoc, 1, globalAmbient);
    glProgramUniform4fv(renderingProgram, ambLoc, 1, lightAmbient);
    glProgramUniform4fv(renderingProgram, diffLoc, 1, lightDiffuse);
    glProgramUniform4fv(renderingProgram, specLoc, 1, lightSpecular);
    glProgramUniform3fv(renderingProgram, posLoc, 1, lightPos);
    glProgramUniform4fv(renderingProgram, mAmbLoc, 1, matAmb);
    glProgramUniform4fv(renderingProgram, mDiffLoc, 1, matDif);
    glProgramUniform4fv(renderingProgram, mSpecLoc, 1, matSpe);
    glProgramUniform1f(renderingProgram, mShiLoc, matShi);
}

void display(GLFWwindow* window, double currentTime) {
    glClear(GL_DEPTH_BUFFER_BIT);//清除深度缓冲区
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renderingProgram);

    // 获取MV矩阵和投影矩阵的统一变量
    mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
    projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
    nLoc = glGetUniformLocation(renderingProgram, "norm_matrix");

    // 构建透视矩阵
    glfwGetFramebufferSize(window, &width, &height);
    aspect = (float)width / (float)height;
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees

    // 构建视图矩阵、模型矩阵和视图-模型矩阵
    vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(LocX, LocY, LocZ));
    mMat *= glm::rotate(glm::mat4(1.0f), (float)currentTime * 0.1f, glm::vec3(1.0, 0.0, 0.0));

    // 通过合并矩阵v和m，创建模型-视图(MV)矩阵，如前
    mvMat = vMat * mMat;

    // 基于当前光源位置，初始化光照
    currentLightPos = glm::vec3(initialLightLoc.x, initialLightLoc.y, initialLightLoc.z);
    installLights(vMat);

    // 构建MV矩阵的逆转置矩阵，以变换法向量
    invTrMat = glm::transpose(glm::inverse(mvMat));

    // 将透视矩阵和MV矩阵复制给相应的统一变量
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));

    // 将VBO关联给顶点着色器中相应的顶点属性
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // 在顶点着色器中，将法向缓冲区(VBO #2)绑定到顶点属性#1
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glVertexAttribPointer(2, 3, GL_FLOAT, false, 0, 0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
    glVertexAttribPointer(3, 3, GL_FLOAT, false, 0, 0);
    glEnableVertexAttribArray(3);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureID);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, NormalTextureID);

    // 调整OpenGL设置，绘制模型
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
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