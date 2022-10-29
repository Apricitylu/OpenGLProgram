#pragma once

#pragma comment(lib,"../../Debug/Common.lib")
#pragma comment(lib,"../../Release/Common.lib")

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>

#include "../../Common/utils.h"

using namespace std;

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];

float x = 0.0f;	//��������x���λ��
float inc = 0.01f;	//�ƶ������ε�ƫ����


void init(GLFWwindow* window) 
{
	const char* vp = "Source/vertShader.glsl";
	const char* fp = "Source/fragShader.glsl";
	renderingProgram = Utils::createShaderProgram(vp, fp);

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
	//glPointSize(30.0f);//���ô�СΪ30���صĵ�
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//�����߿�ģ��

	x += inc;	// �л��������������ƶ�
	if (x > 1.0f) inc = -0.01f;		// ��x���ƶ�������
	if (x < -1.0f) inc = 0.01f;		// �л����������������ƶ�

	GLuint offsetLoc = glGetUniformLocation(renderingProgram, "offset");	// ��ȡ"offset"ָ��
	glProgramUniform1f(renderingProgram, offsetLoc, x);	// ��"x"�е�ֵ����"offset"

	glDrawArrays(GL_TRIANGLES, 0, 3);//�������ߴ������
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
	GLFWwindow* window = glfwCreateWindow(600, 600, "HelloWorld", NULL, NULL);
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