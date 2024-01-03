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
#include "../../Common/Torus.h"


using namespace std;

#define numVAOs 1
#define numVBOs 4

float cameraX, cameraY, cameraZ;
float LocX, LocY, LocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// Ϊdisplay() �����������
GLuint mvLoc, projLoc, nLoc;

int width, height;
float aspect;

// ��ɫ��ͳһ�����е�λ��
GLuint globalAmbLoc, ambLoc, diffLoc, specLoc, posLoc, mAmbLoc, mDiffLoc, mSpecLoc, mShiLoc;

glm::mat4 pMat, vMat, mMat, mvMat, invTrMat;
glm::vec3 currentLightPos, lightPosV;   // ��ģ�ͺ��Ӿ��ռ��еĹ���λ�ã�Vector3f����
float lightPos[3];                      // ����λ�õĸ�������

// ��ʼ������λ��
glm::vec3 initialLightLoc = glm::vec3(5.0f, 2.0f, 2.0f);

// �׹�����
float globalAmbient[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
float lightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

// �ƽ��������
float* matAmb = Utils::goldAmbient();
float* matDif = Utils::goldDiffuse();
float* matSpe = Utils::goldSpecular();
float matShi = Utils::goldShininess();

const int noiseWidth = 256;
const int noiseHeight = 256;
const int noiseDepth = 256;
double noise[noiseWidth][noiseHeight][noiseDepth];
int stripeTexture;

Torus myTorus(0.5f, 0.2f, 64);

void setupVertices(void) {
    std::vector<int> ind = myTorus.getIndices();
    std::vector<glm::vec3> vert = myTorus.getVertices();
    std::vector<glm::vec2> tex = myTorus.getTexCoords();
    std::vector<glm::vec3> norm = myTorus.getNormals();

    std::vector<float> pvalues;
    std::vector<float> tvalues;
    std::vector<float> nvalues;

    int numVertices = myTorus.getNumVertices();
    for (int i = 0; i < numVertices; i++) {
        pvalues.push_back(vert[i].x);
        pvalues.push_back(vert[i].y);
        pvalues.push_back(vert[i].z);

        tvalues.push_back(tex[i].s);
        tvalues.push_back(tex[i].t);

        nvalues.push_back(norm[i].x);
        nvalues.push_back(norm[i].y);
        nvalues.push_back(norm[i].z);
    }
    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(4, vbo);           // ����ǰһ������VBO��������һ����������

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);          // ����λ��
    glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);          // ��������
    glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);          // ������
    glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);  // ����
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);
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

double turbulence(double x, double y, double z, double maxZoom)
{
    double sum = 0.0, zoom = maxZoom;
    while (zoom >= 1.0) {                  // ���һ���ǵ�zoom = 1ʱ
       // ����ƽ���������ͼ�ļ�Ȩ��
        sum = sum + smoothNoise(x / zoom, y / zoom, z / zoom) * zoom;
        zoom = zoom / 2.0;                  // ��ÿ��2���ݵ���������
    }
    sum = 128.0 * sum / maxZoom;           // �Բ�����64��maxZoomֵ����֤RGB < 256
    return sum;
}

// ������generate3Dpattern()������ͼ��������ɫ����ɫ��RGBֵ������ֽ�����
void fillDataArray(GLubyte data[])
{
    double veinFrequency = 2.0;//������������
    double turbPower = 1.5;//���������е��Ŷ�������������Ϊ0��ʹ���Ʋ��ܸ��ţ�
    double turbSize = 64.0;//������������ʱʹ�õ�����ϵ��
    for (int i = 0; i < noiseWidth; i++) {
        for (int j = 0; j < noiseHeight; j++) {
            for (int k = 0; k < noiseDepth; k++) {
                double xyzValue = (float)i / noiseWidth + (float)j / noiseHeight + (float)k / noiseDepth + turbPower * turbulence(i, j, k, turbSize) / 256.0;
                double sineValue = abs(sin(xyzValue * 3.14159 * veinFrequency));

                //��ģ��
                float redPortion = 255.0f * (float)sineValue;
                float greenPortion = 255.0f * (float)min(sineValue * 1.5 - 0.25, 1.0);
                float bluePortion = 255.0f * (float)sineValue;

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

void installLights(glm::mat4 vMatrix) {
    // ����Դλ��ת��Ϊ��ͼ�ռ����꣬�����븡������
    lightPosV = glm::vec3(vMatrix * glm::vec4(currentLightPos, 1.0));
    lightPos[0] = lightPosV.x;
    lightPos[1] = lightPosV.y;
    lightPos[2] = lightPosV.z;

    // ����ɫ���л�ȡ��Դλ�úͲ�������
    globalAmbLoc = glGetUniformLocation(renderingProgram, "globalAmbient");
    ambLoc = glGetUniformLocation(renderingProgram, "light.ambient");
    diffLoc = glGetUniformLocation(renderingProgram, "light.diffuse");
    specLoc = glGetUniformLocation(renderingProgram, "light.specular");
    posLoc = glGetUniformLocation(renderingProgram, "light.position");
    mAmbLoc = glGetUniformLocation(renderingProgram, "material.ambient");
    mDiffLoc = glGetUniformLocation(renderingProgram, "material.diffuse");
    mSpecLoc = glGetUniformLocation(renderingProgram, "material.specular");
    mShiLoc = glGetUniformLocation(renderingProgram, "material.shininess");

    // ����ɫ����Ϊ��Դ�����ͳһ������ֵ
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
    mMat *= glm::rotate(glm::mat4(1.0f), (float)currentTime * 0.1f, glm::vec3(1.0, 0.0, 0.0));

    // ͨ���ϲ�����v��m������ģ��-��ͼ(MV)������ǰ
    mvMat = vMat * mMat;

    // ���ڵ�ǰ��Դλ�ã���ʼ������
    currentLightPos = glm::vec3(initialLightLoc.x, initialLightLoc.y, initialLightLoc.z);
    installLights(vMat);

    // ����MV�������ת�þ����Ա任������
    invTrMat = glm::transpose(glm::inverse(mvMat));

    // ��MV��PROJ�Լ���ת��(������)��������Ӧ��ͳһ����
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));

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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
    glDrawElements(GL_TRIANGLES, myTorus.getNumIndices(), GL_UNSIGNED_INT, 0);
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