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
#include <random>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "../../Common/utils.h"

using namespace std;

#define numVAOs 1
#define numVBOs 2

float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// ������ display() ������ʹ�õı����ռ䣬�������ǾͲ�������Ⱦ�����з���
GLuint mLoc, vLoc, mvLoc, projLoc, tfLoc;
int width, height;
float aspect, timeFactor;
glm::mat4 pMat, vMat, mMat, mvMat, tMat, rMat;

const int noiseWidth = 256;
const int noiseHeight = 256;
const int noiseDepth = 256;
double noise[noiseWidth][noiseHeight][noiseDepth];
int stripeTexture;

void setupVertices(void) {    // 36�����㣬12�������Σ�����˷�����ԭ�㴦��2��2��2������
    float vertexPositions[108] = {
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
    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs, vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
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
    double zoom = 32.0;      // ��������
    int index = 0;
    for (int i = 0; i < noiseWidth; i++) {
        for (int j = 0; j < noiseHeight; j++) {
            for (int k = 0; k < noiseDepth; k++) {
                /*
                * �򵥵�����
                data[i * (noiseWidth * noiseHeight * 4) + j * (noiseHeight * 4) + k * 4 + 0] = (GLubyte)(noise[i][j][k] * 255);
                data[i * (noiseWidth * noiseHeight * 4) + j * (noiseHeight * 4) + k * 4 + 1] = (GLubyte)(noise[i][j][k] * 255);
                data[i * (noiseWidth * noiseHeight * 4) + j * (noiseHeight * 4) + k * 4 + 2] = (GLubyte)(noise[i][j][k] * 255);
                data[i * (noiseWidth * noiseHeight * 4) + j * (noiseHeight * 4) + k * 4 + 3] = (GLubyte)255;
                */

                /*
                * �����������ӣ�ʹ�õ���3D���������ٵس��֡���״��
                data[i * (noiseWidth * noiseHeight * 4) + j * (noiseHeight * 4) + k * 4 + 0] = (GLubyte)(noise[i / zoom][j / zoom][k / zoom] * 255);
                data[i * (noiseWidth * noiseHeight * 4) + j * (noiseHeight * 4) + k * 4 + 1] = (GLubyte)(noise[i / zoom][j / zoom][k / zoom] * 255);
                data[i * (noiseWidth * noiseHeight * 4) + j * (noiseHeight * 4) + k * 4 + 2] = (GLubyte)(noise[i / zoom][j / zoom][k / zoom] * 255);
                data[i * (noiseWidth * noiseHeight * 4) + j * (noiseHeight * 4) + k * 4 + 3] = (GLubyte)255;
                */

                /*
                * ͨ����ÿ����ɢ�Ҷ���ɫֵ��ֵ����һ���Ҷ���ɫֵ��ƽ���ض�������ͼ�ڵġ���ЧӦ��
                data[i * (noiseWidth * noiseHeight * 4) + j * (noiseHeight * 4) + k * 4 + 0] = (GLubyte)(smoothNoise(i / zoom, j / zoom, k / zoom) * 255);
                data[i * (noiseWidth * noiseHeight * 4) + j * (noiseHeight * 4) + k * 4 + 1] = (GLubyte)(smoothNoise(i / zoom, j / zoom, k / zoom) * 255);
                data[i * (noiseWidth * noiseHeight * 4) + j * (noiseHeight * 4) + k * 4 + 2] = (GLubyte)(smoothNoise(i / zoom, j / zoom, k / zoom) * 255);
                data[i * (noiseWidth * noiseHeight * 4) + j * (noiseHeight * 4) + k * 4 + 3] = (GLubyte)255;
                */

                /*��ϸ����������ӵ�ƽ������ͼ*/
                data[i * (noiseWidth * noiseHeight * 4) + j * (noiseHeight * 4) + k * 4 + 0] = (GLubyte)turbulence(i, j, k, zoom);
                data[i * (noiseWidth * noiseHeight * 4) + j * (noiseHeight * 4) + k * 4 + 1] = (GLubyte)turbulence(i, j, k, zoom);
                data[i * (noiseWidth * noiseHeight * 4) + j * (noiseHeight * 4) + k * 4 + 2] = (GLubyte)turbulence(i, j, k, zoom);
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
    cameraX = 0.0f; cameraY = 0.0f; cameraZ = 8.0f;
    cubeLocX = 0.0f; cubeLocY = -2.0f; cubeLocZ = 0.0f; // ��Y��������չʾ͸��
    setupVertices();

    generateNoise();                   // 3Dͼ��������ֻ����һ�Σ�������init()����
    stripeTexture = load3DTexture();       // Ϊ3D����������ͼ��ID
}

void display(GLFWwindow* window, double currentTime) {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renderingProgram);

    // ��ȡMV�����ͶӰ�����ͳһ����
    mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
    projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

    // ����͸�Ӿ���
    glfwGetFramebufferSize(window, &width, &height);
    aspect = (float)width / (float)height;
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees

    // ������ͼ����ģ�;������ͼ-ģ�;���
    vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
    // ʹ�õ�ǰʱ��������x��y��z�Ĳ�ͬ�任
    tMat = glm::translate(glm::mat4(1.0f), glm::vec3(sin(0.35f * currentTime) * 2.0f, cos(0.52f * currentTime) * 2.0f, sin(0.7f * currentTime) * 2.0f));
    rMat = glm::rotate(glm::mat4(1.0f), 1.75f * (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
    rMat = glm::rotate(rMat, 1.75f * (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f));
    rMat = glm::rotate(rMat, 1.75f * (float)currentTime, glm::vec3(0.0f, 0.0f, 1.0f));
    // ��1.75��������ת�ٶ�
    mMat = tMat * rMat;
    mvMat = vMat * mMat;

    // ��͸�Ӿ����MV�����Ƹ���Ӧ��ͳһ����
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

    // ��VBO������������ɫ������Ӧ�Ķ�������
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // ����OpenGL���ã�����ģ��
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, stripeTexture);//ָ������������GL_TEXTURE_3D
    glDrawArrays(GL_TRIANGLES, 0, 36);
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