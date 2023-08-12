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

#include "../../Common/utils.h"

using namespace std;

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];

float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;

GLuint mvpLoc;
int width, height;
float aspect, timeFactor;
glm::mat4 pMat, vMat, mMat, mvpMat;

GLuint brickTexture;


void init(GLFWwindow* window)
{
	const char* vp = "Source/vertShader.glsl";
	const char* cp = "Source/tessCShader.glsl";
	const char* ep = "Source/tessEShader.glsl";
	const char* fp = "Source/fragShader.glsl";
	renderingProgram = Utils::createShaderProgram(vp, cp, ep, fp);

	cameraX = 0.0f; cameraY = 0.5f; cameraZ = 1.5f;
	cubeLocX = 0.0f; cubeLocY = 0.0f; cubeLocZ = 0.0f;

	brickTexture = Utils::loadTexture("texture.jpg");

	//��ʹӦ�ó�����ȫû���õ��κλ�������
	//OpenGL��Ȼ��Ҫ��ʹ����ɫ����ʱ��������һ�������õ�VAO��
	//������������������OpenGLҪ���VAO��
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
}

void display(GLFWwindow* window, double currentTime)
{
	glClear(GL_DEPTH_BUFFER_BIT);	//�����Ȼ�����
	glClearColor(0.0, 0.0, 0.0, 1.0);	//ָ�����������ʱ�õ���ɫֵ
	glClear(GL_COLOR_BUFFER_BIT);	//ʵ��ʹ�ú�ɫ����ɫ�������������

	glUseProgram(renderingProgram);	//�����������ѱ�����ɫ���ĳ�������OpenGL���߽׶�

	// ��ȡMV�����ͶӰ�����ͳһ����
	mvpLoc = glGetUniformLocation(renderingProgram, "mvp_matrix");

	// ����͸�Ӿ���
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees

	// ������ͼ����ģ�;������ͼ-ģ�;���
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ)) * glm::rotate(glm::mat4(1.0f), 0.15f * (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
	mvpMat = pMat * vMat * mMat;

	// ��͸�Ӿ����MV�����Ƹ���Ӧ��ͳһ����
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvpMat));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brickTexture);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LEQUAL);
	glFrontFace(GL_CCW);

	glPatchParameteri(GL_PATCH_VERTICES, 16);			// ÿ�������Ķ������� = 16
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);			// �����߿�ģ��
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_PATCHES, 0, 16);					// �����������������16 x 1������= 16
}

int main(int argc, char** argv)
{
	//��ʼ��GLFW��
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//ָ���˻���������OpenGL�汾4.3����
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//ʵ����GLFWwindow
	GLFWwindow* window = glfwCreateWindow(600, 600, "Tessellation", NULL, NULL);
	glfwMakeContextCurrent(window);//��window�뵱ǰOpenGL�����Ĺ�������

	//��ʼ��GLEW�⡪��GLFW����Ĭ����˫�����
	if (glewInit() != GLEW_OK)
	{
		exit(EXIT_FAILURE);
	}

	//������ֱͬ����Vsync��
	glfwSwapInterval(1);

	//����һ��init()����
	init(window);

	//�ظ�����display()����
	while (!glfwWindowShouldClose(window))
	{
		display(window, glfwGetTime());	//glfwGetTime()�����᷵��GLFW��ʼ��֮�󾭹���ʱ��
		glfwSwapBuffers(window);//������Ļ
		glfwPollEvents();//����������¼����簴���¼���
	}

	//֪ͨGLFW���ٴ����Լ���ֹ����
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);

	return 0;
}