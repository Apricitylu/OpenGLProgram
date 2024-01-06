#pragma once

//����Debugģʽ��Releaseģʽ
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

// Ϊdisplay() �����������
GLuint mvLoc, projLoc, nLoc, tLoc, dLoc;

int width, height;
float aspect;

// ��ɫ��ͳһ�����е�λ��
GLuint globalAmbLoc, ambLoc, diffLoc, specLoc, posLoc, mAmbLoc, mDiffLoc, mSpecLoc, mShiLoc;

glm::mat4 pMat, vMat, mMat, mvMat, invTrMat;
glm::vec3 currentLightPos, lightPosV;   // ��ģ�ͺ��Ӿ��ռ��еĹ���λ�ã�Vector3f����

const int noiseWidth = 256;
const int noiseHeight = 256;
const int noiseDepth = 256;
double noise[noiseWidth][noiseHeight][noiseDepth];
int stripeTexture;

double rotAmt = 0.0;         // �������ƿ�����Ư�Ƶ�Y����ת��
float depth = 0.01f;         // 3D����ͼ����Ȳ��ң�����ʹ���𽥱仯

Sphere mySphere(48);

void setupVertices(void) {
    std::vector<int> ind = mySphere.getIndices();
    std::vector<glm::vec3> vert = mySphere.getVertices();
    std::vector<glm::vec2> tex = mySphere.getTexCoords();
    std::vector<glm::vec3> norm = mySphere.getNormals();

    std::vector<float> pvalues;     // ����λ��
    std::vector<float> tvalues;     // ��������
    std::vector<float> nvalues;     // ������

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

    // �Ѷ�����뻺���� #0
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

    // ������������뻺���� #1
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

    // �ѷ��������뻺���� #2
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);
}

/*
* ͨ��������Ӧԭʼ����״������ͼ��������Χ��8���Ҷ�ֵ�ļ�Ȩƽ��ֵ�������������ͼ��ƽ���汾�е�ÿ�����صĻҶ�ֵ��
*/
double smoothNoise(double x1, double y1, double z1)
{
    // x1��y1��z1��С�����֣����ڵ�ǰ���أ��ӵ�ǰ�鵽��һ����İٷֱȣ�
    double fractX = x1 - (int)x1;
    double fractY = y1 - (int)y1;
    double fractZ = z1 - (int)z1;

    // ��X��Y��Z�����ϵ��������ص�����
    int x2 = ((int)x1 + noiseWidth + 1) % noiseWidth;
    int y2 = ((int)y1 + noiseHeight + 1) % noiseHeight;
    int z2 = ((int)z1 + noiseDepth + 1) % noiseDepth;

    // ͨ������3���᷽���ֵ�Ҷȣ�ƽ������
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

//Logistic����ʹ��ɫ�������ڰ�ɫ����ɫ�������ǽ�������֮���ֵ���Ӷ��������и��಻ͬ�Ʊ߽���Ӿ�Ч����
double logistic(double x) {
    double k = 0.2;     // ��΢���������ʳ̶ȣ������������ٵķ������Ʊ߽�
    return (1.0 / (1.0 + pow(2.718, -k * x)));
}

double turbulence(double x, double y, double z, double size) {
    double value = 0.0, initialSize = size, cloudQuant;//cloudQuant��������ͼ�а�ɫ���������ɫ���������;
    while (size >= 0.9) {
        value = value + smoothNoise(x / size, y / size, z / size) * size;
        size = size / 2.0;
    }
    cloudQuant = 110.0;          // ��΢����������
    value = value / initialSize;
    value = 256.0 * logistic(value * 128.0 - cloudQuant);
    return value;
}

// ������generate3Dpattern()������ͼ��������ɫ����ɫ��RGBֵ������ֽ�����
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
// �������Ƶ�3Dͼ��
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

// ��˳���ֽ�����������ؽ��������, �䷽ʽ������ǰ��5.12
int load3DTexture() {
    GLuint textureID;
    GLubyte* data = new GLubyte[noiseWidth * noiseHeight * noiseDepth * 4];

    fillDataArray(data);//ͼ�����ݱ���ʽ��Ϊ��Ӧ��RGBA��ɫ�������ֽ�����

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
    LocX = 0.0f; LocY = 0.0f; LocZ = 0.0f; // ��Y��������չʾ͸��
    setupVertices();

    generateNoise();                   // 3Dͼ��������ֻ����һ�Σ�������init()����
    stripeTexture = load3DTexture();       // Ϊ3D����������ͼ��ID
}

void display(GLFWwindow* window, double currentTime) {
    glClear(GL_DEPTH_BUFFER_BIT);//�����Ȼ�����
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renderingProgram);

    // ����ģ��-��ͼ�任��ͶӰ�Լ���ת��(������)�����ͳһ����
    mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
    projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
    nLoc = glGetUniformLocation(renderingProgram, "norm_matrix");

    // ����͸�Ӿ���
    glfwGetFramebufferSize(window, &width, &height);
    aspect = (float)width / (float)height;
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees

    // ������ͼ����ģ�;������ͼ-ģ�;���
    vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(LocX, LocY, LocZ));
    mMat *= glm::rotate(glm::mat4(1.0f), (float)currentTime * 0.01f, glm::vec3(0.0, 1.0, 0.0));

    // ͨ���ϲ�����v��m������ģ��-��ͼ(MV)������ǰ
    mvMat = vMat * mMat;

    // ��MV��PROJ�Լ���ת��(������)��������Ӧ��ͳһ����
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

    // ���޸ĵ������������꣬��ʹ�Ʊ仯
    dLoc = glGetUniformLocation(renderingProgram, "d");
    depth += 0.00008f;
    if (depth >= 0.99f) depth = 0.01f;           // �����ǵ���������ͼ���յ�ʱ���ؿ�ͷ
    glUniform1f(dLoc, depth);

    // �ڶ�����ɫ���У������㻺����(VBO #0)�󶨵���������#0
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // �ڶ�����ɫ���У������򻺳���(VBO #2)�󶨵���������#1
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);
    glEnableVertexAttribArray(1);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, stripeTexture);//ָ������������GL_TEXTURE_3D

    glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());
}

int main(void) {                            // main()��֮ǰ��û�б仯
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