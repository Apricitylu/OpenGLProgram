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

	//即使应用程序完全没有用到任何缓冲区，
	//OpenGL仍然需要在使用着色器的时候至少有一个创建好的VAO，
	//所以这两行用来创建OpenGL要求的VAO。
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
}

void display(GLFWwindow* window, double currentTime)
{
	glClear(GL_DEPTH_BUFFER_BIT);	//清除深度缓冲区
	glClearColor(0.0, 0.0, 0.0, 1.0);	//指定了清除背景时用的颜色值
	glClear(GL_COLOR_BUFFER_BIT);	//实际使用红色对颜色缓冲区进行填充

	glUseProgram(renderingProgram);	//将含有两个已编译着色器的程序载入OpenGL管线阶段

	// 获取MV矩阵和投影矩阵的统一变量
	mvpLoc = glGetUniformLocation(renderingProgram, "mvp_matrix");

	// 构建透视矩阵
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees

	// 构建视图矩阵、模型矩阵和视图-模型矩阵
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ)) * glm::rotate(glm::mat4(1.0f), 0.15f * (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
	mvpMat = pMat * vMat * mMat;

	// 将透视矩阵和MV矩阵复制给相应的统一变量
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvpMat));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brickTexture);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LEQUAL);
	glFrontFace(GL_CCW);

	glPatchParameteri(GL_PATCH_VERTICES, 16);			// 每个补丁的顶点数量 = 16
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);			// 呈现线框模型
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_PATCHES, 0, 16);					// 补丁顶点的总数量：16 x 1个补丁= 16
}

int main(int argc, char** argv)
{
	//初始化GLFW库
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//指定了机器必须与OpenGL版本4.3兼容
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//实例化GLFWwindow
	GLFWwindow* window = glfwCreateWindow(600, 600, "Tessellation", NULL, NULL);
	glfwMakeContextCurrent(window);//将window与当前OpenGL上下文关联起来

	//初始化GLEW库——GLFW窗口默认是双缓冲的
	if (glewInit() != GLEW_OK)
	{
		exit(EXIT_FAILURE);
	}

	//开启垂直同步（Vsync）
	glfwSwapInterval(1);

	//调用一次init()函数
	init(window);

	//重复调用display()函数
	while (!glfwWindowShouldClose(window))
	{
		display(window, glfwGetTime());	//glfwGetTime()，它会返回GLFW初始化之后经过的时间
		glfwSwapBuffers(window);//绘制屏幕
		glfwPollEvents();//处理窗口相关事件（如按键事件）
	}

	//通知GLFW销毁窗口以及终止运行
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);

	return 0;
}