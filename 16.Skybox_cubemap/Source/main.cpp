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

// ������ display() ������ʹ�õı����ռ䣬�������ǾͲ�������Ⱦ�����з���
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
     1.00f, 0.66f, 1.00f, 0.33f, 0.75f, 0.33f,    // �������½�
     0.75f, 0.33f, 0.75f, 0.66f, 1.00f, 0.66f,    // �������Ͻ�
     0.75f, 0.33f, 0.50f, 0.33f, 0.75f, 0.66f,    // �������½�
     0.50f, 0.33f, 0.50f, 0.66f, 0.75f, 0.66f,    // �������Ͻ�
     0.50f, 0.33f, 0.25f, 0.33f, 0.50f, 0.66f,    // �������½�
     0.25f, 0.33f, 0.25f, 0.66f, 0.50f, 0.66f,    // �������Ͻ�
     0.25f, 0.33f, 0.00f, 0.33f, 0.25f, 0.66f,    // �������½�
     0.00f, 0.33f, 0.00f, 0.66f, 0.25f, 0.66f,    // �������Ͻ�
     0.25f, 0.33f, 0.50f, 0.33f, 0.50f, 0.00f,    // �������½�
     0.50f, 0.00f, 0.25f, 0.00f, 0.25f, 0.33f,    // �������Ͻ�
     0.25f, 1.00f, 0.50f, 1.00f, 0.50f, 0.66f,    // �������½�
     0.50f, 0.66f, 0.25f, 0.66f, 0.25f, 1.00f     // �������Ͻ�
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
        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,   // ǰ����
        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,   // �Ҳ���
        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,   // �����
        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,   // �����
        0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,   // �����������1
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f    // �����������2
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
    pyramidLocX = 2.0f; pyramidLocY = 2.0f; pyramidLocZ = 0.0f; // ��Y��������չʾ͸��

    brickTexture = Utils::loadTexture("brick1.jpg");

    skyboxTexture = Utils::loadCubeMap("skybox_cubemap");         // ������պ�������ļ���
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);//������������ڵı��Լ��ٻ������ӷ�

    setupVertices();
}

void display(GLFWwindow* window, double currentTime) {
    glClear(GL_DEPTH_BUFFER_BIT);//�����Ȼ�����
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(renderingProgramCubeMap);

    // ����͸�Ӿ���
    glfwGetFramebufferSize(window, &width, &height);
    aspect = (float)width / (float)height;
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees

    //��պ�
    // ��ȡMV�����ͶӰ�����ͳһ����
    mvLoc = glGetUniformLocation(renderingProgramCubeMap, "v_matrix");
    projLoc = glGetUniformLocation(renderingProgramCubeMap, "proj_matrix");
   
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

    // ����ͼ���������ջ
    vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

    // ׼�����Ȼ�����պС�M������պз����������λ��
    //mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cameraX, cameraY, cameraZ));

    // ����MODEL-VIEW����
    //mvMat = vMat * mMat;

    // ��ǰ����MV��PROJ�������ͳһ����
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(vMat));

    // ���ð�������Ļ�����
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // ���ð�����������Ļ�����
    //glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    //glEnableVertexAttribArray(1);

    //������պ�����
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);    // ���������˳����˳ʱ��ģ������Ǵ��ڲ��鿴�����ʹ����ʱ�����˳��GL_CCW

    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 36);       // ��û����Ȳ��Ե�����»�����պ�
    glEnable(GL_DEPTH_TEST);

    
    //����
    glUseProgram(renderingProgram);

    // ��ȡMV�����ͶӰ�����ͳһ����
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

int main(void) {                            // main()��֮ǰ��û�б仯
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