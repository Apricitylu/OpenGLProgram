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

// 为display() 函数分配变量
GLuint mvLoc, projLoc, nLoc, tLoc, dLoc;

int width, height;
float aspect;

// 着色器统一变量中的位置
GLuint globalAmbLoc, ambLoc, diffLoc, specLoc, posLoc, mAmbLoc, mDiffLoc, mSpecLoc, mShiLoc;

glm::mat4 pMat, vMat, mMat, mvMat, invTrMat;
glm::vec3 currentLightPos, lightPosV;   // 在模型和视觉空间中的光照位置，Vector3f类型

const int noiseWidth = 256;
const int noiseHeight = 256;
const int noiseDepth = 256;
double noise[noiseWidth][noiseHeight][noiseDepth];
int stripeTexture;

double rotAmt = 0.0;         // 用来让云看起来漂移的Y轴旋转量
float depth = 0.01f;         // 3D噪声图的深度查找，用来使云逐渐变化

Sphere mySphere(48);

void setupVertices(void) {
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

/*
* 通过计算相应原始“块状”噪声图中纹素周围的8个灰度值的加权平均值来计算给定噪声图的平滑版本中的每个纹素的灰度值。
*/
double smoothNoise(double x1, double y1, double z1)
{
    // x1、y1和z1的小数部分（对于当前纹素，从当前块到下一个块的百分比）
    double fractX = x1 - (int)x1;
    double fractY = y1 - (int)y1;
    double fractZ = z1 - (int)z1;

    // 在X、Y和Z方向上的相邻像素的索引
    int x2 = ((int)x1 + noiseWidth + 1) % noiseWidth;
    int y2 = ((int)y1 + noiseHeight + 1) % noiseHeight;
    int z2 = ((int)z1 + noiseDepth + 1) % noiseDepth;

    // 通过按照3个轴方向插值灰度，平滑噪声
    double value = 0.0;
    value += (1 - fractX) * (1 - fractY) * (1 - fractZ) * noise[(int)x1][(int)y1][(int)z1];
    value += (1 - fractX) * fractY * (1 - fractZ) * noise[(int)x1][(int)y2][(int)z1];
    value += fractX * (1 - fractY) * (1 - fractZ) * noise[(int)x2][(int)y1][(int)z1];
    value += fractX * fractY * (1 - fractZ) * noise[(int)x2][(int)y2][(int)z1];

    value += (1 - fractX) * (1 - fractY) * fractZ * noise[(int)x1][(int)y1][(int)z2];
    value += (1 - fractX) * fractY * fractZ * noise[(int)x1][(int)y2][(int)z2];
    value += fractX * (1 - fractY) * fractZ * noise[(int)x2][(int)y1][(int)z2];
    value += fractX * fractY * fractZ * noise[(int)x2][(int)y2][(int)z2];
    return value;
}

//Logistic函数使颜色更倾向于白色或蓝色，而不是介于两者之间的值，从而产生具有更多不同云边界的视觉效果。
double logistic(double x) {
    double k = 0.2;     // 可微调的云朦胧程度，产生更多或更少的分明的云边界
    return (1.0 / (1.0 + pow(2.718, -k * x)));
}

double turbulence(double x, double y, double z, double size) {
    double value = 0.0, initialSize = size, cloudQuant;//cloudQuant调整噪声图中白色（相对于蓝色）的相对量;
    while (size >= 0.9) {
        value = value + smoothNoise(x / size, y / size, z / size) * size;
        size = size / 2.0;
    }
    cloudQuant = 110.0;          // 可微调的云质量
    value = value / initialSize;
    value = 256.0 * logistic(value * 128.0 - cloudQuant);
    return value;
}

// 按照由generate3Dpattern()构建的图案，用蓝色、黄色的RGB值来填充字节数组
void fillDataArray(GLubyte data[])
{
    for (int i = 0; i < noiseWidth; i++) {
        for (int j = 0; j < noiseHeight; j++) {
            for (int k = 0; k < noiseDepth; k++) {
                float brightness = 1.0f - (float)turbulence(i, j, k, 32) / 256.0f;
                float redPortion = brightness * 255.0f;
                float greenPortion = brightness * 255.0f;
                float bluePortion = 1.0f * 255.0f;
                data[i * (noiseWidth * noiseHeight * 4) + j * (noiseHeight * 4) + k * 4 + 0] = (GLubyte)redPortion;
                data[i * (noiseWidth * noiseHeight * 4) + j * (noiseHeight * 4) + k * 4 + 1] = (GLubyte)greenPortion;
                data[i * (noiseWidth * noiseHeight * 4) + j * (noiseHeight * 4) + k * 4 + 2] = (GLubyte)bluePortion;
                data[i * (noiseWidth * noiseHeight * 4) + j * (noiseHeight * 4) + k * 4 + 3] = (GLubyte)255;
            }
        }
    }
}
// 构建条纹的3D图案
void generateNoise()
{
    int xStep = 0, yStep = 0, zStep = 0, sumSteps = 0;
    for (int x = 0; x < noiseWidth; x++) {
        for (int y = 0; y < noiseHeight; y++) {
            for (int z = 0; z < noiseDepth; z++) {
                noise[x][y][z] = (double)rand() / (RAND_MAX + 1.0);
            }
        }
    }
}

// 将顺序字节数据数组加载进纹理对象, 其方式类似于前面5.12
int load3DTexture() {
    GLuint textureID;
    GLubyte* data = new GLubyte[noiseWidth * noiseHeight * noiseDepth * 4];

    fillDataArray(data);//图像数据被格式化为对应于RGBA颜色分量的字节序列

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_3D, textureID);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexStorage3D(GL_TEXTURE_3D, 1, GL_RGBA8, noiseWidth, noiseHeight, noiseDepth);
    glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, noiseWidth, noiseHeight, noiseDepth, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, data);
    return textureID;
}

void init(GLFWwindow* window) {
    const char* vp = "Source/vertShader.glsl";
    const char* fp = "Source/fragShader.glsl";
    renderingProgram = Utils::createShaderProgram(vp, fp);
    cameraX = 0.0f; cameraY = 0.0f; cameraZ = 2.0f;
    LocX = 0.0f; LocY = 0.0f; LocZ = 0.0f; // 沿Y轴下移以展示透视
    setupVertices();

    generateNoise();                   // 3D图案和纹理只加载一次，所以在init()里作
    stripeTexture = load3DTexture();       // 为3D纹理保存整型图案ID
}

void display(GLFWwindow* window, double currentTime) {
    glClear(GL_DEPTH_BUFFER_BIT);//清除深度缓冲区
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renderingProgram);

    // 用于模型-视图变换、投影以及逆转置(法向量)矩阵的统一变量
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
    mMat *= glm::rotate(glm::mat4(1.0f), (float)currentTime * 0.01f, glm::vec3(0.0, 1.0, 0.0));

    // 通过合并矩阵v和m，创建模型-视图(MV)矩阵，如前
    mvMat = vMat * mMat;

    // 将MV、PROJ以及逆转置(法向量)矩阵传入相应的统一变量
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

    // 逐渐修改第三个纹理坐标，以使云变化
    dLoc = glGetUniformLocation(renderingProgram, "d");
    depth += 0.00008f;
    if (depth >= 0.99f) depth = 0.01f;           // 当我们到达纹理贴图的终点时返回开头
    glUniform1f(dLoc, depth);

    // 在顶点着色器中，将顶点缓冲区(VBO #0)绑定到顶点属性#0
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // 在顶点着色器中，将法向缓冲区(VBO #2)绑定到顶点属性#1
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);
    glEnableVertexAttribArray(1);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, stripeTexture);//指定了纹理类型GL_TEXTURE_3D

    glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());
}

int main(void) {                            // main()和之前的没有变化
    if (!glfwInit()) { exit(EXIT_FAILURE); }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(600, 600, "Demo", NULL, NULL);
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