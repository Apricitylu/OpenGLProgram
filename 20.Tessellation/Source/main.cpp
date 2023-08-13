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


void init(GLFWwindow* window)
{
	const char* vp = "Source/vertShader.glsl";
	const char* cp = "Source/tessCShader.glsl";
	const char* ep = "Source/tessEShader.glsl";
	const char* fp = "Source/fragShader.glsl";
	renderingProgram = Utils::createShaderProgram(vp, cp, ep, fp);

	cameraX = 0.0f; cameraY = 0.5f; cameraZ = 2.0f;
	cubeLocX = -0.5f; cubeLocY = 0.0f; cubeLocZ = 0.0f;

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
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));
	mvpMat = pMat * vMat * mMat;

	// ��͸�Ӿ����MV�����Ƹ���Ӧ��ͳһ����
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvpMat));

	glPatchParameteri(GL_PATCH_VERTICES, 1);	//ָ��һ����Ƶ��ж��������
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	//�����߿�ģ��
	glDrawArrays(GL_PATCHES, 0, 1);		//GL_PATCHES��Ψһ������ͼԪ����
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
		glfwPollEvents();//������������¼����簴���¼���
	}

	//֪ͨGLFW���ٴ����Լ���ֹ����
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);

	return 0;
}