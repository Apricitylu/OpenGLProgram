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

float x = 0.0f;	//三角形在x轴的位置
float inc = 0.01f;	//移动三角形的偏移量


void init(GLFWwindow* window) 
{
	const char* vp = "Source/vertShader.glsl";
	const char* fp = "Source/fragShader.glsl";
	renderingProgram = Utils::createShaderProgram(vp, fp);

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
	//glPointSize(30.0f);//设置大小为30像素的点
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//呈现线框模型

	x += inc;	// 切换让三角形左右移动
	if (x > 1.0f) inc = -0.01f;		// 沿x轴移动三角形
	if (x < -1.0f) inc = 0.01f;		// 切换至让三角形向左移动

	GLuint offsetLoc = glGetUniformLocation(renderingProgram, "offset");	// 获取"offset"指针
	glProgramUniform1f(renderingProgram, offsetLoc, x);	// 将"x"中的值传给"offset"

	glDrawArrays(GL_TRIANGLES, 0, 3);//启动管线处理过程
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
	GLFWwindow* window = glfwCreateWindow(600, 600, "HelloWorld", NULL, NULL);
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